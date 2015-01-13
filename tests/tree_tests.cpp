#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tree test
#include <boost/test/unit_test.hpp>
#include "../tree/tree.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE(tree_test)
{
  Tree tree1("(S (A a) (B b))");
  BOOST_CHECK_EQUAL(tree1.symbol, "S");
  BOOST_CHECK_EQUAL(tree1.children[0]->symbol, "A");
  BOOST_CHECK_EQUAL(tree1.to_str(), "(S (A a)(B b))");
}

BOOST_AUTO_TEST_CASE(tree_test2)
{
  string s = "(S (A a) (B b))(";
  Tree t;
  BOOST_CHECK_THROW(t.parse(s), ParseException);
}
