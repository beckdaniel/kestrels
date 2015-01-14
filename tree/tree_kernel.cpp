#include <boost/foreach.hpp>
#include <iostream>
#include "tree_kernel.hpp"

using namespace std;

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<float> &lambda, 
							 bool normalize){
  this->normalize = normalize;
  this->lambda = vector<float>(lambda);
  this->alpha.push_back(0.0);
}

SymbolAwareSubsetTreeKernel::SymbolAwareSubsetTreeKernel(const vector<float> &lambda, 
							 const vector<float> &alpha,
							 bool normalize){
  this->normalize = normalize;
  this->lambda = vector<float>(lambda);
  this->alpha = vector<float>(alpha);
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
  kernel_result.k = 6.0;
  kernel_result.dlambda = {0.0};
  kernel_result.dalpha = {0.0};
}



void SymbolAwareSubsetTreeKernel::delta(const IDPair &id_pair, const NodeList &nodes1,
					const NodeList &nodes2, double* delta_matrix,
					double* dlambda_tensor, double* dalpha_tensor){

}
