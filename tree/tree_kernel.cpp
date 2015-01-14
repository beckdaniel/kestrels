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
					vector<float> &result){
  //this->build_cache(trees);
  BOOST_FOREACH(string tree_repr, trees){
    cout << tree_repr << endl;
    result.push_back(6.0);
    //this->
  }
}


void SymbolAwareSubsetTreeKernel::compute_kernel(const NodeList &nodes1,
						 const NodeList &nodes2,
						 KernelResult &kernel_result){

}



void SymbolAwareSubsetTreeKernel::delta(const IDPair &id_pair, const NodeList &nodes1,
					const NodeList &nodes2, double* delta_matrix,
					double* dlambda_tensor, double* dsigma_tensor){

}
