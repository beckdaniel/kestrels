#include <boost/regex.hpp>
#include <boost/format.hpp> 
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "tree.hpp"

using namespace std;
using namespace boost;

string Node::to_str(){
  stringstream result;
  result << this->production;
  result << " " << this->node_id << " [ ";
  BOOST_FOREACH( int ch_id, this->children_ids )
    result << ch_id << " ";
  result << "]";
  return result.str();
}

bool compare_nodes(const Node* node1, const Node* node2){
  return (node1->production < node2->production);
}

Tree::Tree(){}

Tree::Tree(const string& s){
  this->parse(s);
}

string Tree::to_str(){
  if (this->children.size() == 0) return this->symbol;
  stringstream result;
  result << "(";
  result << this->symbol;
  BOOST_FOREACH( Tree* child, this->children ){
    result << " ";
    result << child->to_str();
  }
  result << ")";
  return result.str();
}

void Tree::parse(const string& s){
  // This is inspired on NLTK parse method
  string open_pattern = "\\(";
  string close_pattern = "\\)";
  string node_pattern = str(format("[^\\s%s%s]+") % open_pattern % close_pattern);
  string leaf_pattern = str(format("[^\\s%s%s]+") % open_pattern % close_pattern);
  string token_pattern;
  token_pattern = str(format("%s\\s*(%s)?|%s|(%s)") % open_pattern % node_pattern
		      % close_pattern % leaf_pattern);
  regex token_re(token_pattern);
  vector<Tree*> stack;
  Tree* t =  new Tree();
  t->symbol = "START";
  stack.push_back(t);

  string::const_iterator start, end;
  start = s.begin();
  end = s.end();   
  match_results<string::const_iterator> what;
  string label;
  string token;

  while (regex_search(start, end, what, token_re)){
    token = string(what[0].first, what[0].second);
      
    // Beginning of a tree/subtree
    if (token[0] == '('){
      if ((stack.size() == 1) && (stack[0]->children.size() > 0))
	throw ParseException(str(format("Error on the beginning of a tree. Token: %s") % token));
      label = token.substr(1);
      trim(label);
      t = new Tree();
      t->symbol = label;
      stack.push_back(t);
    }

    // End of a tree/subtree
    else if (token == ")"){
      if (stack.size() == 1)
	throw ParseException(str(format("Error on the end of a tree. Token: %s") % token));
      t = stack.back();
      stack.pop_back();
      stack.back()->children.push_back(t);
    }

    // Leaf node
    else {
      if (stack.size() == 1)
	throw ParseException(str(format("Error on a leaf node. Token: %s") % token));
      trim(token);
      t = new Tree();
      t->symbol = token;
      stack.back()->children.push_back(t);
    }

    // update search position:
    start = what[0].second;
  }

  // check if we have only one complete tree.
  if ((stack.size() > 1) || (stack[0]->children.size() == 0))
    throw ParseException("Error on the end of parsing");
  else if ((stack[0]->symbol != "START") || (stack[0]->children.size() != 1))
    throw ParseException("Error on the end of parsing");

  this->symbol = stack[0]->children[0]->symbol;
  this->children = vector<Tree*>(stack[0]->children[0]->children);
  delete t;
}

void Tree::get_node_list(NodeList& node_list, 
			 map<string, int>& lambda_buckets,
			 map<string, int>& alpha_buckets){
  //NodeList node_list;
  int temp = this->add_node(this, node_list, lambda_buckets, alpha_buckets);
  std::sort(node_list.begin(), node_list.end(), compare_nodes);
  map<int, int> ids_converter;
  for (int i = 0; i < node_list.size(); i++)
    ids_converter[node_list[i]->node_id] = i;
  BOOST_FOREACH( Node* node, node_list ) {
    node->node_id = 0;
    for (int i = 0; i < node->children_ids.size(); i++) {
      node->children_ids[i] = ids_converter[node->children_ids[i]];
    }
  }
  //return node_list;
}

int Tree::add_node(const Tree* tree, NodeList& node_list,
		   map<string, int>& lambda_buckets,
		   map<string, int>& alpha_buckets){
  if (tree->children.size() == 0)
    return -1;
  stringstream production;
  production << tree->symbol;
  Node* node = new Node();
  try {
    node->lambda_index = lambda_buckets.at(tree->symbol);
  } catch (out_of_range) {
    node->lambda_index = 0;
  }
  try {
    node->alpha_index = alpha_buckets.at(tree->symbol);
  } catch (out_of_range) {
    node->alpha_index = 0;
  }
  int ch_id;
  BOOST_FOREACH( Tree* child, tree->children ){
    production << " " << child->symbol;
    ch_id = this->add_node(child, node_list, lambda_buckets,
			   alpha_buckets);
    if (ch_id != -1)
      node->children_ids.push_back(ch_id);
  }
  node->production = production.str();
  node->node_id = node_list.size();
  node_list.push_back(node);
  return node->node_id;
}
