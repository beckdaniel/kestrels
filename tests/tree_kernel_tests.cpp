#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tree_kernel test
#include <boost/test/unit_test.hpp>
#include "../tree/tree_kernel.hpp"

#define SASSTK SymbolAwareSubsetTreeKernel

using namespace std;

struct SimpleTree{
  SimpleTree() {
    trees.push_back("(S (AA a) (B b))");
  };
  ~SimpleTree() {};

  vector<string> trees;
  vector<float> result;
};

BOOST_FIXTURE_TEST_CASE(kdiag1, SimpleTree){
  vector<float> lambda = {1.0};
  SASSTK kernel = SASSTK(lambda, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_EQUAL(result[0], 6);
};

BOOST_FIXTURE_TEST_CASE(kdiag2, SimpleTree){
  vector<float> lambda = {1.0, 0.5};
  vector<float> alpha = {0.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_EQUAL(result[0], 3);
}

BOOST_FIXTURE_TEST_CASE(kdiag3, SimpleTree){
  vector<float> lambda = {1.0, 0.5};
  vector<float> alpha = {0.2, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_EQUAL(result[0], 3.44);
}

BOOST_FIXTURE_TEST_CASE(kdiag4, SimpleTree){
  vector<float> lambda = {0.6, 0.5};
  vector<float> alpha = {1.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_EQUAL(result[0], 2.736);
}
