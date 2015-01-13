#include <boost/regex.hpp>
#include <boost/format.hpp> 
#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include "tree.hpp"

using namespace std;
using namespace boost;

Tree::Tree(){}

Tree::Tree(const string& s){
  this->parse(s);
}

string Tree::to_str(){
  if (this->children.size() == 0) return this->symbol;
  stringstream result;
  result << "(";
  result << this->symbol;
  result << " ";
  vector<Tree*>::iterator it;
  for (it = this->children.begin() ; it != this->children.end(); it++){
    result << (*it)->to_str();
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
}
