#include <boost/foreach.hpp>
#include <iostream>
#include "tree_kernel.hpp"

using namespace std;

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<double> &lambda, 
							 const bool normalize){
  this->lambda = vector<double>(lambda);
  this->alpha.push_back(1.0);
  this->normalize = normalize;
}

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<double> &lambda, 
							 const vector<double> &alpha,
							 const bool normalize){
  this->lambda = vector<double>(lambda);
  this->alpha = vector<double>(alpha);
  this->normalize = normalize;
}

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<double>& lambda,
							 const vector<double>& alpha,
							 const bool normalize,
							 const map<string, int>& lambda_buckets){
  this->lambda = vector<double>(lambda);
  this->alpha = vector<double>(alpha);
  this->normalize = normalize;
  this->lambda_buckets = map<string, int>(lambda_buckets);
}

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<double>& lambda,
							 const vector<double>& alpha,
							 const bool normalize,
							 const map<string, int>& lambda_buckets,
							 const map<string, int>& alpha_buckets){
  this->lambda = vector<double>(lambda);
  this->alpha = vector<double>(alpha);
  this->normalize = normalize;
  this->lambda_buckets = map<string, int>(lambda_buckets);
  this->alpha_buckets = map<string, int>(alpha_buckets);
}


void SymbolAwareSubsetTreeKernel::build_cache(const vector<string> &trees){
  BOOST_FOREACH(string tree_repr, trees){
    try {
      this->tree_cache.at(tree_repr);
    } catch (out_of_range) {
      Tree tree = Tree(tree_repr);
      NodeList nodes;
      tree.get_node_list(nodes, this->lambda_buckets);
      this->tree_cache[tree_repr] = nodes;
    }
  }
}


void SymbolAwareSubsetTreeKernel::Kdiag(const vector<string>& trees,
					vector<KernelResult>& result){
  this->build_cache(trees);
  BOOST_FOREACH(string tree_repr, trees){
    result.push_back(KernelResult(this->lambda.size(), this->alpha.size()));
    NodeList nodes = this->tree_cache[tree_repr];
    this->compute_kernel(nodes, nodes, result.back());
  }
}


void SymbolAwareSubsetTreeKernel::compute_kernel(const NodeList& nodes1,
						 const NodeList& nodes2,
						 KernelResult& kernel_result){
  int len1 = nodes1.size();
  int len2 = nodes2.size();
  int lambda_size = this->lambda.size();
  int alpha_size = this->alpha.size();
  int matrix_size = sizeof(double) * len1 * len2;
  double* delta_matrix = (double*) malloc(matrix_size);
  double* dlambda_tensor = (double*) malloc(matrix_size * lambda_size);
  double* dalpha_tensor = (double*) malloc(matrix_size * alpha_size);
  int index;
  for (int i = 0; i < len1; ++i){
    for (int j = 0; j < len2; ++j){
      index = i * len1 + j;
      delta_matrix[index] = 0;
      for (int k = 0; k < lambda_size; ++k)
	dlambda_tensor[index * lambda_size + k] = 0;
      for (int k = 0; k < alpha_size; ++k)
	dalpha_tensor[index * alpha_size + k] = 0;
    }
  }
  vector<IDPair> id_pairs;
  this->get_node_pairs(nodes1, nodes2, id_pairs);
  KernelResult temp_result = KernelResult(lambda_size, alpha_size);
  BOOST_FOREACH(IDPair id_pair, id_pairs){
    this->delta(id_pair, nodes1, nodes2, kernel_result, temp_result,
		delta_matrix, dlambda_tensor, dalpha_tensor);
  }
  free(delta_matrix);
  free(dlambda_tensor);
  free(dalpha_tensor);
}


void SymbolAwareSubsetTreeKernel::get_node_pairs(const NodeList& nodes1,
						 const NodeList& nodes2,
						 vector<IDPair>& id_pairs){
  int len1 = nodes1.size();
  int len2 = nodes2.size();
  int i1 = 0;
  int i2 = 0;
  int reset;
  Node* n1;
  Node* n2;
  while (true){
    if ((i1 >= len1) || (i2 >= len2))
      return;
    n1 = nodes1[i1];
    n2 = nodes2[i2];
    if (n1->production > n2->production)
      i2 += 1;
    else if (n1->production < n2->production)
      i1 += 1;
    else {
      while (n1->production == n2->production){
	reset = i2;
	while (n1->production == n2->production){
	  id_pairs.push_back(IDPair({i1, i2}));
	  i2 += 1;
	  if (i2 >= len2)
	    break;
	  n2 = nodes2[i2];
	}
	i1 += 1;
	if (i1 >= len1)
	  return;
	i2 = reset;
	n1 = nodes1[i1];
	n2 = nodes2[i2];
      }
    }
  }
}

void SymbolAwareSubsetTreeKernel::delta(const IDPair& id_pair, const NodeList& nodes1,
					const NodeList& nodes2, KernelResult& kernel_result,
					KernelResult& temp_result, double* delta_matrix,
					double* dlambda_tensor,	double* dalpha_tensor){
  int id1 = id_pair.first;
  int id2 = id_pair.second;
  int len2 = nodes2.size();
  int lambda_size = this->lambda.size();
  int alpha_size = this->alpha.size();
  int index = id1 * len2 + id2;
  double val = delta_matrix[index];

  // RECURSIVE CASE: get value from DP matrix if it was calculated before
  if (val > 0){
    temp_result.k = val;
    for (int i = 0; i < lambda_size; ++i)
      temp_result.dlambda[i] = dlambda_tensor[index * lambda_size + i];
    for (int i = 0; i < alpha_size; ++i)
      temp_result.dalpha[i] = dalpha_tensor[index * alpha_size + i];
    return;
  }

  // BASE CASE: found a preterminal
  Node* node1 = nodes1[id1];
  int lambda_index = node1->lambda_index;
  if (node1->children_ids.empty()){
    delta_matrix[index] = this->lambda[lambda_index];
    temp_result.k = this->lambda[lambda_index];
    kernel_result.k += this->lambda[lambda_index];
    for (int i = 0; i < lambda_size; ++i){
      if (i == lambda_index){
	temp_result.dlambda[i] = 1;
	kernel_result.dlambda[i] += 1;
	dlambda_tensor[index * lambda_size + i] = 1;
      } else {
	temp_result.dlambda[i] = 0;
      }
    }
    temp_result.dalpha.assign(alpha_size, 0);
    return;
  }
  
  // RECURSIVE CASE: if val == 0, then we proceed the recursion
  Node* node2 = nodes2[id2];
  double prod = 1;
  double sum_lambda = 0;
  double sum_alpha = 0;
  double g = 1;
  double denom;
  vector<double> vec_lambda;
  vector<double> vec_alpha;
  vec_lambda.assign(lambda_size, 0);
  vec_alpha.assign(alpha_size, 0);
  ChildrenIDs children1 = node1->children_ids;
  ChildrenIDs children2 = node2->children_ids;
  //int alpha_index = node1.alpha_index; // TODO
  int alpha_index = 0;
  IDPair child_pair;
  for (int i = 0; i < children1.size(); ++i){
    child_pair = {children1[i], children2[i]};
    if (nodes1[child_pair.first]->production == nodes2[child_pair.second]->production){
      this->delta(child_pair, nodes1, nodes2, kernel_result, temp_result,
		  delta_matrix, dlambda_tensor, dalpha_tensor);
      denom = this->alpha[alpha_index] + temp_result.k;
      g *= denom;
      for (int j = 0; j < lambda_size; ++j)
	vec_lambda[j] += (temp_result.dlambda[j] / denom);
      for (int j = 0; j < alpha_size; ++j){
	if (j == alpha_index)
	  vec_alpha[j] += ((1 + temp_result.dalpha[j]) / denom);
	else
	  vec_alpha[j] += (temp_result.dalpha[j] / denom);
      }
    } else {
      g *= this->alpha[alpha_index];
      vec_alpha[alpha_index] += 1 / this->alpha[alpha_index];
    }
  }
  double delta_result, dlambda_result, dalpha_result;
  delta_result = this->lambda[lambda_index] * g;
  delta_matrix[index] = delta_result;
  temp_result.k = delta_result;
  kernel_result.k += delta_result;
  for (int i = 0; i < lambda_size; ++i){
    dlambda_result = delta_result * vec_lambda[i];
    if (i == lambda_index)
      dlambda_result += g;
    dlambda_tensor[index * lambda_size + i] = dlambda_result;
    temp_result.dlambda[i] = dlambda_result;
    kernel_result.dlambda[i] += dlambda_result;
  } 
  for (int i = 0; i < alpha_size; ++i){
    dalpha_result = delta_result * vec_alpha[i];
    dalpha_tensor[index * alpha_size + i] = dalpha_result;
    temp_result.dalpha[i] = dalpha_result;
    kernel_result.dalpha[i] += dalpha_result;
  }
}

