#ifndef TREE_KERNEL_H
#define TREE_KERNEL_H
#include <vector>
#include <string>

class SymbolAwareSubsetTreeKernel{
public:
  bool normalize;

  SymbolAwareSubsetTreeKernel(const std::vector<float> &lambda, bool normalize);

  std::vector<float> Kdiag(const std::vector<std::string> &trees);
private:

};

#endif //TREE_KERNEL_H
