#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <vector>
#include <iostream>
#include "tree.hpp"

using namespace std;
using namespace boost;

Tree::Tree(){}

Tree::Tree(const string& s){
  this->parse(s);
}

void Tree::parse(const string& s){
  this->symbol = s[1];

  // This is inspired on NLTK parse regex
  string open_pattern = "\\(";
  string close_pattern = "\\)";
  string node_pattern = str(format("[^\\s%s%s]+") % open_pattern % close_pattern);
  string leaf_pattern = str(format("[^\\s%s%s]+") % open_pattern % close_pattern);
  string token_pattern;
  token_pattern = str(format("%s\\s*(%s)?|%s|(%s)") % open_pattern % node_pattern
		      % close_pattern % leaf_pattern);
  //regex token_re("\\(\\s*([^\\s\\(\\)]+)?|\\)|([^\\s\\(\\)]+)");
  regex token_re(token_pattern);
  vector<Tree> stack;

  string::const_iterator start, end;
  start = s.begin();
  end = s.end();   
  match_results<string::const_iterator> what;
  string label;
  string token;
  while(regex_search(start, end, what, token_re)){
    token = string(what[0].first, what[0].second);
    cout << token << endl;
      
    // Beginning of a tree/subtree
    if (token[0] == "\\("){
      label= token.substr(1);
    }


    // update search position:
    start = what[0].second;
  }
}
