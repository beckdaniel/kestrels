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

vector<float> SymbolAwareSubsetTreeKernel::Kdiag(const vector<string> &trees){
  vector<float> result;
  BOOST_FOREACH(string tree_repr, trees){
    cout << tree_repr << endl;
    result.push_back(6.0);
  }
  return result;
}
