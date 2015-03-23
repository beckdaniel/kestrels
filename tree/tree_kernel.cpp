#include <boost/foreach.hpp>
#include <iostream>
#include <cmath>
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

void SymbolAwareSubsetTreeKernel::Kdiag(const vector<string>& trees,
					vector<KernelResult>& result){
  this->build_cache(trees);
  BOOST_FOREACH(string tree_repr, trees){
    result.push_back(KernelResult(this->lambda.size(), this->alpha.size()));
    NodeList nodes = this->tree_cache[tree_repr];
    this->compute_kernel(nodes, nodes, result.back());
  }
}

void SymbolAwareSubsetTreeKernel::K(const vector<string>& trees,
				    vector<VecResult>& result){
  vector<string> empty;
  this->K(trees, empty, result);
}

void SymbolAwareSubsetTreeKernel::K(const vector<string>& trees1,
				    const vector<string>& trees2,
				    vector<VecResult>& result){
  // Build caches and check if we are calculating a gram matrix.
  int size1 = trees1.size();
  int size2;
  this->build_cache(trees1);
  bool gram = false;
  if (trees2.empty()){
    gram = true;
    size2 = trees1.size();
  }
  else {
    this->build_cache(trees2);
    size2 = trees2.size();
  }

  // Allocate the result matrix
  result.assign(size1, VecResult());
  for (int i = 0; i < size1; ++i)
    result[i].assign(size2, KernelResult(this->lambda.size(), this->alpha.size()));

  //return;

  // Obtain the diagonal values for normalization
  VecResult diags1, diags2;
  this->Kdiag(trees1, diags1);
  if (gram) {
    diags2 = VecResult(diags1);
  } else {
    this->Kdiag(trees2, diags2);
  }

  // Iterate over the two sets of trees
  for (int i = 0; i < size1; ++i){
    for (int j = 0; j < size2; ++j){
      if (gram){
	if (i < j) // symmetric matrix
	  continue;
	if ((i == j) && (this->normalize)) {
	  result[i][j].k = 1.0;
	  result[i][j].dlambda.assign(this->lambda.size(), 0.0);
	  result[i][j].dalpha.assign(this->alpha.size(), 0.0);
	  continue;
	}
      }

      cout << "BEFORE KERNEL COMPUTATION" << endl;
      cout << trees1[i] << endl;
      cout << trees2[j] << endl;

      NodeList nodes1 = this->tree_cache[trees1[i]];
      NodeList nodes2 = this->tree_cache[trees2[j]];
      this->compute_kernel(nodes1, nodes2, result[i][j]);

      cout << result[i][j].k << endl;

      if (this->normalize)
	this->compute_normalization(diags1[i], diags2[j], result[i][j]);
      if (gram) { // symmetric matrix
	result[j][i].k = result[i][j].k;
	result[j][i].dlambda = vector<double>(result[i][j].dlambda);
	result[j][i].dalpha = vector<double>(result[i][j].dalpha);
      }
    }
  }
}


void SymbolAwareSubsetTreeKernel::build_cache(const vector<string>& trees){
  BOOST_FOREACH(string tree_repr, trees){
    try {
      this->tree_cache.at(tree_repr);
    } catch (out_of_range) {
      Tree tree = Tree(tree_repr);
      NodeList nodes;
      tree.get_node_list(nodes, this->lambda_buckets, this->alpha_buckets);
      this->tree_cache[tree_repr] = nodes;
    }
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
  int alpha_index = node1->alpha_index;
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

void SymbolAwareSubsetTreeKernel::compute_normalization(const KernelResult& diag_i,
							const KernelResult& diag_j,
							KernelResult& norm_result){
  double norm = diag_i.k * diag_j.k;
  double sqrt_norm = sqrt(norm);
  norm_result.k /= sqrt_norm;
  double diff_lambda, diff_alpha;
  for (int k = 0; k < this->lambda.size(); ++k){
    diff_lambda = ((diag_i.dlambda[k] * diag_j.k) +
		   (diag_i.k * diag_j.dlambda[k]));
    diff_lambda /= (2 * norm);
    norm_result.dlambda[k] = ((norm_result.dlambda[k] / sqrt_norm) -
			      (norm_result.k * diff_lambda));
  }
  for (int k = 0; k < this->alpha.size(); ++k){
    diff_alpha = ((diag_i.dalpha[k] * diag_j.k) +
		   (diag_i.k * diag_j.dalpha[k]));
    diff_alpha /= (2 * norm);
    norm_result.dalpha[k] = ((norm_result.dalpha[k] / sqrt_norm) -
			      (norm_result.k * diff_alpha));
  }
}
