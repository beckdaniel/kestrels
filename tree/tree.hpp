#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <exception>

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

};

#endif //TREE_H
