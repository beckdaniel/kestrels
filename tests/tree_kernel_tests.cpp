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
  //vector<float> lambda;
};

BOOST_FIXTURE_TEST_CASE(kdiag_tests, SimpleTree)
{
  vector<float> lambda = {1.0};
  //lambda.push_back(1.0);
  SASSTK kernel = SASSTK(lambda, false);
  BOOST_CHECK_EQUAL(kernel.Kdiag(trees)[0], 6);
}
