#ifndef TREE_KERNEL_H
#define TREE_KERNEL_H
#include <vector>
#include <string>
#include <unordered_map>
#include "tree.hpp"

typedef std::pair<int, int> IDPair;

struct KernelResult{
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

  SymbolAwareSubsetTreeKernel(const std::vector<float>& lambda, bool normalize);
  SymbolAwareSubsetTreeKernel(const std::vector<float>& lambda,
			      const std::vector<float>& alpha, bool normalize);
  void Kdiag(const std::vector<std::string>& trees,
	     std::vector<KernelResult>& result);
  void compute_kernel(const NodeList& nodes1, const NodeList& nodes2,
		      KernelResult& kernel_result);


private:
  void build_cache(const std::vector<std::string>& trees);
  void delta(const IDPair& id_pair, const NodeList& nodes1,
	     const NodeList& nodes2, double* delta_matrix,
	     double* dlambda_tensor, double* dsigma_tensor);
};

#endif //TREE_KERNEL_H
