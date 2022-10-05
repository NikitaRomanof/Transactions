.PHONY:  all clean check tests test gcov_report dvi install uninstall dist

CXX= g++ -std=c++17 -lstdc++ -Wall -Wextra -Werror
CFLAGS= -fprofile-arcs -ftest-coverage
GFLAGS= -lgtest
TEST_FLAGS= -lgtest -lgtest_main -lpthread
HASH_FILES= hash_table.cc
TREE_FILES= self_balancing_binary_search_tree.cc
ALL_FILES= main.cc consol.cc controller.cc hash_table.cc self_balancing_binary_search_tree.cc
GCOV_FILES= hash_table.cc self_balancing_binary_search_tree.cc hash_test.cc tree_test.cc

all: clean hash_table.a self_balancing_binary_search_tree.a test console

tests: test_hash test_tree

test: tests

test_hash:
	$(CXX) -g hash_test.cc hash_table.cc -o tst.out -lgtest
	./tst.out

test_tree:
	$(CXX) -g tree_test.cc self_balancing_binary_search_tree.cc -o tst.out -lgtest
	./tst.out

hash_table.a:
	$(CXX) -c $(HASH_FILES)
	ar rcs hash_table.a hash_table.o

self_balancing_binary_search_tree.a:
	$(CXX) -c $(TREE_FILES)
	ar rcs self_balancing_binary_search_tree.a self_balancing_binary_search_tree.o

clean:
	rm -rf test *.a *.o *.cfg *.gcda *.gcno *.html *.out *.info *.dSYM report

console:
	$(CXX) $(ALL_FILES) -o transactions.out
	./transactions.out

format:
	clang-format -i *.cc *.cpp *.inl *.h
	clang-format -n *.cc

check: 
	cp ../materials/linters/suppressions.cfg suppressions.cfg
	cppcheck -q --std=c++17 --enable=all --language=c++ --suppressions-list=suppressions.cfg *.cc *.cpp *.inl *.h

gcov_report: clean
	$(CXX) $(CFLAGS) tree_test.cc self_balancing_binary_search_tree.cc $(TEST_FLAGS) -o gcov_report1.out
	./gcov_report1.out
	$(CXX) $(CFLAGS) hash_test.cc hash_table.cc $(TEST_FLAGS) -o gcov_report2.out
	./gcov_report2.out
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html
