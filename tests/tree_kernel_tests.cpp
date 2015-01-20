#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tree_kernel test
#include <boost/test/unit_test.hpp>
#include "../tree/tree_kernel.hpp"
#include <vector>
#include <map>

#define SASSTK SymbolAwareSubsetTreeKernel
#define TOLERANCE 0.0001

using namespace std;

/****************************
Tests using Kdiag on a simple tree
****************************/

struct SimpleTree{
  SimpleTree() {
    trees.push_back("(S (AA a) (B b))");
    tol = TOLERANCE;
  };
  ~SimpleTree() {};

  vector<string> trees;
  vector<KernelResult> result;
  double tol;
};

BOOST_FIXTURE_TEST_CASE(kdiag1, SimpleTree){
  vector<double> lambda = {1.0};
  SASSTK kernel = SASSTK(lambda, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 6, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag2, SimpleTree){
  vector<double> lambda = {1.0, 0.5};
  vector<double> alpha = {0.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 3, tol);
}

BOOST_FIXTURE_TEST_CASE(kdiag3, SimpleTree){
  vector<double> lambda = {1.0, 0.5};
  vector<double> alpha = {0.2, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 3.44, tol);
}

BOOST_FIXTURE_TEST_CASE(kdiag4, SimpleTree){
  vector<double> lambda = {0.6, 0.5};
  vector<double> alpha = {1.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 2.736, tol);
}

BOOST_FIXTURE_TEST_CASE(kdiag5, SimpleTree){
  vector<double> lambda = {0.2, 0.5};
  vector<double> alpha = {1.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 0.688, tol);
}

BOOST_FIXTURE_TEST_CASE(kdiag_buckets1, SimpleTree){
  vector<double> lambda = {1.0, 0.6};
  vector<double> alpha = {1.0, 1.0};
  map<string, int> lambda_buckets {{"AA", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.8, tol);
}

BOOST_FIXTURE_TEST_CASE(kdiag_buckets2, SimpleTree){
  vector<double> lambda = {1.0, 0.6, 0.2};
  vector<double> alpha = {1.0};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 2.72, tol);
}

BOOST_FIXTURE_TEST_CASE(kdiag_buckets3, SimpleTree){
  vector<double> lambda = {1.0, 0.6, 0.2, 1.0};
  vector<double> alpha = {1.0};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}, {"S", 3}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 2.72, tol);
}

/****************************
Tests using Kdiag on a not so simple tree
****************************/

struct SimpleTree2{
  SimpleTree2() {
    trees.push_back("(S (AA (AA a)) (B b))");
    tol = TOLERANCE;
  };
  ~SimpleTree2() {};

  vector<string> trees;
  vector<KernelResult> result;
  double tol;
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_1, SimpleTree2){
  vector<double> lambda = {1.0};
  SASSTK kernel = SASSTK(lambda, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 10, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_2, SimpleTree2){
  vector<double> lambda = {1.0, 0.5};
  vector<double> alpha = {0.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_3, SimpleTree2){
  vector<double> lambda = {1.0};
  vector<double> alpha = {0.2};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.88, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_4, SimpleTree2){
  vector<double> lambda = {0.6, 0.5};
  vector<double> alpha = {1.0, 1.0};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.0416, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets1, SimpleTree){
  vector<double> lambda = {1.0, 0.6};
  vector<double> alpha = {1.0, 1.0};
  map<string, int> lambda_buckets {{"AA", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 6.48, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets2, SimpleTree){
  vector<double> lambda = {1.0, 0.6, 0.2};
  vector<double> alpha = {1.0};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.112, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets3, SimpleTree){
  vector<double> lambda = {1.0, 0.6, 0.2, 1};
  vector<double> alpha = {1.0};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}, {"S", 3}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.112, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets4, SimpleTree){
  vector<double> lambda = {1.0, 0.6, 0.2};
  vector<double> alpha = {1.0, 0.3};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}};
  map<string, int> alpha_buckets {{"S", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets, alpha_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 2.39, tol);
};
