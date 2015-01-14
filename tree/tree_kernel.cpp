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


void SymbolAwareSubsetTreeKernel::Kdiag(const vector<string> &trees,
					vector<KernelResult> &result){
  this->build_cache(trees);
  BOOST_FOREACH(string tree_repr, trees){
    result.push_back(KernelResult());
    NodeList nodes = this->tree_cache[tree_repr];
    this->compute_kernel(nodes, nodes, result.back());
  }
}


void SymbolAwareSubsetTreeKernel::compute_kernel(const NodeList &nodes1,
						 const NodeList &nodes2,
						 KernelResult &kernel_result){
  // DEBUG
  kernel_result.k = 6.0;
  kernel_result.dlambda = {0.0};
  kernel_result.dalpha = {0.0};
  // END DEBUG
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
  free(delta_matrix);
  free(dlambda_tensor);
  free(dalpha_tensor);
}



void SymbolAwareSubsetTreeKernel::delta(const IDPair &id_pair, const NodeList &nodes1,
					const NodeList &nodes2, double* delta_matrix,
					double* dlambda_tensor, double* dalpha_tensor){

}
