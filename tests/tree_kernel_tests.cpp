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

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets1, SimpleTree2){
  vector<double> lambda = {1.0, 0.6};
  vector<double> alpha = {1.0, 1.0};
  map<string, int> lambda_buckets {{"AA", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 6.48, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets2, SimpleTree2){
  vector<double> lambda = {1.0, 0.6, 0.2};
  vector<double> alpha = {1.0};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.112, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets3, SimpleTree2){
  vector<double> lambda = {1.0, 0.6, 0.2, 1};
  vector<double> alpha = {1.0};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}, {"S", 3}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 4.112, tol);
};

BOOST_FIXTURE_TEST_CASE(kdiag_2_buckets4, SimpleTree2){
  vector<double> lambda = {1.0, 0.6, 0.2};
  vector<double> alpha = {1.0, 0.3};
  map<string, int> lambda_buckets {{"AA", 1}, {"B", 2}};
  map<string, int> alpha_buckets {{"S", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets, alpha_buckets);
  kernel.Kdiag(trees, result);
  BOOST_CHECK_CLOSE(result[0].k, 2.39, tol);
};

/*************************
Tests for K on a pair of different trees
 *************************/

struct TreePair{
  TreePair() {
    trees1.push_back("(S (AA (AA a)) (B b))");
    trees2.push_back("(S (AA (AA a)) (B c))");
    tol = TOLERANCE;
  };
  ~TreePair() {};

  vector<string> trees1;
  vector<string> trees2;
  vector<VecResult> result;
  double tol;
};

BOOST_FIXTURE_TEST_CASE(K_1, TreePair){
  vector<double> lambda = {1.0};
  SASSTK kernel = SASSTK(lambda, false);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 6, tol);
  BOOST_CHECK_CLOSE(result[0][0].dlambda[0], 10, tol);
  BOOST_CHECK_CLOSE(result[0][0].dalpha[0], 6, tol);
};

BOOST_FIXTURE_TEST_CASE(K_2, TreePair){
  vector<double> lambda = {1.0, 1.0};
  vector<double> alpha = {0.2};
  SASSTK kernel = SASSTK(lambda, alpha, false);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 2.48, tol);
};

BOOST_FIXTURE_TEST_CASE(K_3, TreePair){
  vector<double> lambda = {1.0, 0.5};
  vector<double> alpha = {0.2};
  map<string, int> lambda_buckets {{"AA", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 0.96, tol);
};

BOOST_FIXTURE_TEST_CASE(K_4, TreePair){
  vector<double> lambda = {1.0, 0.5};
  vector<double> alpha = {1.0, 0.2};
  map<string, int> lambda_buckets {{"AA", 1}};
  map<string, int> alpha_buckets {{"AA", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets, alpha_buckets);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 2.2, tol);
};

BOOST_FIXTURE_TEST_CASE(K_5, TreePair){
  vector<double> lambda = {1.0, 0.4};
  vector<double> alpha = {1.0, 0.2};
  map<string, int> lambda_buckets {{"AA", 1}};
  map<string, int> alpha_buckets {{"AA", 1}};
  SASSTK kernel = SASSTK(lambda, alpha, false, lambda_buckets, alpha_buckets);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 1.88, tol);
  BOOST_CHECK_CLOSE(result[0][0].dlambda[0], 1.24, tol);
  BOOST_CHECK_CLOSE(result[0][0].dlambda[1], 3, tol);
  BOOST_CHECK_CLOSE(result[0][0].dalpha[0], 2.24, tol);
  BOOST_CHECK_CLOSE(result[0][0].dalpha[1], 0.8, tol);
};

/***************************
 Tests for the normalized version
 **************************/

BOOST_FIXTURE_TEST_CASE(Knorm_1, TreePair){
  vector<double> lambda = {1.0};
  SASSTK kernel = SASSTK(lambda, true);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 0.6, tol);
  BOOST_CHECK_CLOSE(result[0][0].dlambda[0], -0.2, tol);
  BOOST_CHECK_CLOSE(result[0][0].dalpha[0], 0.12, tol);
};

BOOST_FIXTURE_TEST_CASE(Knorm_2, TreePair){
  vector<double> lambda = {1.0, 0.5};
  SASSTK kernel = SASSTK(lambda, true);
  kernel.K(trees1, trees2, result);
  BOOST_CHECK_CLOSE(result[0][0].k, 0.6, tol);
  BOOST_CHECK_CLOSE(result[0][0].dlambda[0], -0.2, tol);
  BOOST_CHECK_CLOSE(result[0][0].dalpha[0], 0.12, tol);
};
