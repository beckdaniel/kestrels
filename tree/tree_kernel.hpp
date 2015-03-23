#ifndef TREE_KERNEL_H
#define TREE_KERNEL_H
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include "tree.hpp"

typedef std::pair<int, int> IDPair;

struct KernelResult{
  double k;
  std::vector<double> dlambda;
  std::vector<double> dalpha;
  KernelResult(){};
  KernelResult(int lambda_size, int alpha_size){
    this->k = 0.0;
    this->dlambda.assign(lambda_size, 0.0);
    this->dalpha.assign(alpha_size, 0.0);
  };
};

typedef std::vector<KernelResult> VecResult;

class SymbolAwareSubsetTreeKernel{
public:
  bool normalize;
  std::vector<double> lambda;
  std::vector<double> alpha;
  std::map<std::string, int> lambda_buckets;
  std::map<std::string, int> alpha_buckets;

  SymbolAwareSubsetTreeKernel(const std::vector<double>& lambda, const bool normalize);
  SymbolAwareSubsetTreeKernel(const std::vector<double>& lambda,
			      const std::vector<double>& alpha, const bool normalize);
  SymbolAwareSubsetTreeKernel(const std::vector<double>& lambda,
			      const std::vector<double>& alpha, const bool normalize,
			      const std::map<std::string, int>& lambda_buckets);
  SymbolAwareSubsetTreeKernel(const std::vector<double>& lambda,
			      const std::vector<double>& alpha, const bool normalize,
			      const std::map<std::string, int>& lambda_buckets,
			      const std::map<std::string, int>& alpha_buckets);
  void Kdiag(const std::vector<std::string>& trees,
	     std::vector<KernelResult>& result);
  void K(const std::vector<std::string>& trees,
	 std::vector<VecResult>& result);
  void K(const std::vector<std::string>& trees1,
	 const std::vector<std::string>& trees2,
	 std::vector<VecResult>& result);

private:
  std::unordered_map<std::string, NodeList> tree_cache;
  void build_cache(const std::vector<std::string>& trees);
  void compute_kernel(const NodeList& nodes1, const NodeList& nodes2,
		      KernelResult& kernel_result);
  void get_node_pairs(const NodeList& nodes1, const NodeList& nodes2,
		      std::vector<IDPair>& id_pairs);
  void delta(const IDPair& id_pair, const NodeList& nodes1,
	     const NodeList& nodes2, KernelResult& kernel_result,
	     KernelResult& temp_result, double* delta_matrix,
	     double* dlambda_tensor, double* dalpha_tensor);
  void compute_normalization(const KernelResult& diag_i,
			     const KernelResult& diag_j,
			     KernelResult& norm_result);
};

#endif //TREE_KERNEL_H
