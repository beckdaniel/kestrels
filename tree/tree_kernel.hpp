#ifndef TREE_KERNEL_H
#define TREE_KERNEL_H
#include <vector>
#include <string>
#include <unordered_map>
#include "tree.hpp"

typedef std::pair<int, int> IDPair;

struct KernelResult{
  float k;
  std::vector<double> dlambda;
  std::vector<double> dalpha;
};

class SymbolAwareSubsetTreeKernel{
public:
  bool normalize;
  std::vector<double> lambda;
  std::vector<double> alpha;
  std::unordered_map<std::string, NodeList> tree_cache;

  SymbolAwareSubsetTreeKernel(const std::vector<double>& lambda, bool normalize);
  SymbolAwareSubsetTreeKernel(const std::vector<double>& lambda,
			      const std::vector<double>& alpha, bool normalize);
  void Kdiag(const std::vector<std::string>& trees,
	     std::vector<KernelResult>& result);
  void compute_kernel(const NodeList& nodes1, const NodeList& nodes2,
		      KernelResult& kernel_result);


private:
  void build_cache(const std::vector<std::string>& trees);
  void get_node_pairs(const NodeList& nodes1, const NodeList& nodes2,
		      std::vector<IDPair>& id_pairs);
  void delta(const IDPair& id_pair, const NodeList& nodes1,
	     const NodeList& nodes2, double* delta_matrix,
	     double* dlambda_tensor, double* dalpha_tensor);
};

#endif //TREE_KERNEL_H
