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
  BOOST_CHECK_EQUAL(tree1.to_str(), "(S (A a) (B b))");
}

BOOST_AUTO_TEST_CASE(tree_test2)
{
  Tree tree1("(S (NP (ADJ colorless) (N ideas)) (VP (V sleep) (ADV furiously)))");
  BOOST_CHECK_EQUAL(tree1.symbol, "S");
  BOOST_CHECK_EQUAL(tree1.children[0]->symbol, "NP");
  BOOST_CHECK_EQUAL(tree1.to_str(), "(S (NP (ADJ colorless) (N ideas)) (VP (V sleep) (ADV furiously)))");
}

BOOST_AUTO_TEST_CASE(tree_test3)
{
  Tree tree1("(S (NP (ADJ colórless) (N ideas)) (VP (V sleep) (ADV furiously)))");
  BOOST_CHECK_EQUAL(tree1.symbol, "S");
  BOOST_CHECK_EQUAL(tree1.children[0]->symbol, "NP");
  BOOST_CHECK_EQUAL(tree1.to_str(), "(S (NP (ADJ colórless) (N ideas)) (VP (V sleep) (ADV furiously)))");
}

BOOST_AUTO_TEST_CASE(tree_test_exceptions)
{
  Tree t;
  BOOST_CHECK_THROW(t.parse("(S (A a) (B b))("), ParseException);
  BOOST_CHECK_THROW(t.parse("((S (A a) (B b))"), ParseException);
  BOOST_CHECK_THROW(t.parse("(S (A a) (B b)) (S a)"), ParseException);
  BOOST_CHECK_THROW(t.parse("S (A a) (B b)"), ParseException);
  BOOST_CHECK_THROW(t.parse("(S (A (a) (B b))"), ParseException);
}

BOOST_AUTO_TEST_CASE(tree_node_list)
{
  Tree tree1("(S (NP (ADJ colorless) (N ideas)) (VP (V sleep) (ADV furiously)))");
  NodeList node_list;
  node_list.push_back(new Node("ADJ colorless"));
  node_list.push_back(new Node("ADV furiously"));
  node_list.push_back(new Node("N ideas"));
  node_list.push_back(new Node("NP ADJ N"));
  node_list.push_back(new Node("S NP VP"));
  node_list.push_back(new Node("V sleep"));
  node_list.push_back(new Node("VP V AD"));

  node_list[3]->children_ids.push_back(0);
  node_list[4]->children_ids.push_back(3);
  node_list[4]->children_ids.push_back(6);
  node_list[6]->children_ids.push_back(5);
  
  //cout << "BLAH" << endl;
  NodeList result = tree1.get_node_list();
  //cout << "BLAH2" << endl;
  for (int i = 0; i < node_list.size(); i++){
    //cout << i << endl;
    BOOST_CHECK_EQUAL(result[i], node_list[i]);
  }
}
