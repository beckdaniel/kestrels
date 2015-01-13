#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

class Tree{
public:
  std::string symbol;
  std::vector<Tree*> children;

  Tree();
  Tree(const std::string& s);
  void parse(const std::string& s);
};

#endif //TREE_H
