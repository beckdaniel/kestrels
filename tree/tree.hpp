#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <exception>

typedef std::vector<int> ChildrenIDs;

struct Node{
  std::string production;
  int node_id;
  ChildrenIDs children_ids;
  Node(){};
  Node(const std::string& s){this->production = s;};
  std::string to_str();
};

typedef std::vector<Node*> NodeList;

class ParseException: public std::exception{
public:
  ParseException(const std::string m) : msg(m) {}
  ~ParseException() throw() {}
  const char* what() { return msg.c_str(); }

private:
  std::string msg;
};

class Tree{
public:
  std::string symbol;
  std::vector<Tree*> children;

  Tree();
  Tree(const std::string& s);
  std::string to_str();
  void parse(const std::string& s);
  NodeList get_node_list();

private:
  int add_node(const Tree* tree, NodeList& temp_list);

};

#endif //TREE_H
