#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <exception>
#include <map>

typedef std::vector<int> ChildrenIDs;

struct Node{
  std::string production;
  int node_id;
  ChildrenIDs children_ids;
  int lambda_index;
  int alpha_index;
  Node(){};
  Node(const std::string& s){
    this->production = s;
    this->node_id = 0;
    this->lambda_index = 0;
    this->alpha_index = 0;
  };
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
  void get_node_list(NodeList& node_list,
		     std::map<std::string, int>& lambda_buckets,
		     std::map<std::string, int>& alpha_buckets);

private:
  int add_node(const Tree* tree, NodeList& node_list,
	       std::map<std::string, int>& lambda_buckets,
	       std::map<std::string, int>& alpha_buckets);

};

#endif //TREE_H
