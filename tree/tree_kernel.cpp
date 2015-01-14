#include <boost/foreach.hpp>
#include <iostream>
#include "tree_kernel.hpp"

using namespace std;

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<double> &lambda, 
							 bool normalize){
  this->normalize = normalize;
  this->lambda = vector<double>(lambda);
  this->alpha.push_back(0.0);
}

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<double> &lambda, 
							 const vector<double> &alpha,
							 bool normalize){
  this->normalize = normalize;
  this->lambda = vector<double>(lambda);
  this->alpha = vector<double>(alpha);
}

void SymbolAwareSubsetTreeKernel::build_cache(const vector<string> &trees){
  BOOST_FOREACH(string tree_repr, trees){
    try {
      this->tree_cache.at(tree_repr);
    } catch (out_of_range) {
      Tree tree = Tree(tree_repr);
      NodeList nodes;
      tree.get_node_list(nodes);
      this->tree_cache[tree_repr] = nodes;
    }
  }
}


void SymbolAwareSubsetTreeKernel::Kdiag(const vector<string>& trees,
					vector<KernelResult>& result){
  this->build_cache(trees);
  BOOST_FOREACH(string tree_repr, trees){
    result.push_back(KernelResult());
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
  BOOST_FOREACH(IDPair id_pair, id_pairs){
    this->delta(id_pair, nodes1, nodes2, kernel_result, delta_matrix,
		dlambda_tensor, dalpha_tensor);
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
					double* delta_matrix, double* dlambda_tensor,
					double* dalpha_tensor){
  // DEBUG
  kernel_result.k = 6.0;
  kernel_result.dlambda = {0.0};
  kernel_result.dalpha = {0.0};
  cout << id_pair.first << " " << id_pair.second << endl;
  // END DEBUG
}
