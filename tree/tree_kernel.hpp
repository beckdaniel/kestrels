#ifndef TREE_KERNEL_H
#define TREE_KERNEL_H
#include <vector>
#include <string>
#include <unordered_map>
#include "tree.hpp"

class KernelResult{
  float k;
  std::vector<float> dlambda;
  std::vector<float> dsigma;
};

class SymbolAwareSubsetTreeKernel{
public:
  bool normalize;
  std::vector<float> lambda;
  std::vector<float> alpha;
  std::unordered_map<std::string, NodeList> tree_cache;

  SymbolAwareSubsetTreeKernel(const std::vector<float> &lambda, bool normalize);
  SymbolAwareSubsetTreeKernel(const std::vector<float> &lambda,
			      const std::vector<float> &alpha, bool normalize);
  std::vector<float> Kdiag(const std::vector<std::string> &trees);
  void compute_kernel(const NodeList &node_list1, const NodeList &node_list2,
		      KernelResult &kernel_result);

private:
  void build_cache(const std::vector<std::string> &trees);

};

#endif //TREE_KERNEL_H
