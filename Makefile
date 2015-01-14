tree_test: tests/tree_tests.cpp tree/tree.cpp tree/tree.hpp
	g++ tests/tree_tests.cpp tree/tree.cpp -lboost_unit_test_framework -lboost_regex -std=c++11
tree_kernel_test: tests/tree_kernel_tests.cpp tree/*
	g++ tests/tree_kernel_tests.cpp tree/*.cpp -lboost_unit_test_framework -lboost_regex -std=c++11
