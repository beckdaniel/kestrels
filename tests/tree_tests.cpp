#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tree test
#include <boost/test/unit_test.hpp>
#include "../tree/tree.hpp"

BOOST_AUTO_TEST_CASE(tree_test)
{
  Tree tree1("(S (A a) (B b)");
  BOOST_CHECK_EQUAL(tree1.symbol, "S");
  BOOST_CHECK_EQUAL(tree1.children[0]->symbol, "A");
}
