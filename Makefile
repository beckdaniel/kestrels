test: tests/tree_tests.cpp tree/tree.cpp
	g++ tests/tree_tests.cpp tree/tree.cpp -lboost_unit_test_framework -lboost_regex
