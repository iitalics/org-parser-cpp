CXX=clang++ -std=c++1z
CXXFLAGS=-Wall -g -O1

.PHONY: test
test:
	$(CXX) $(CXXFLAGS) $(wildcard test/*.cpp) -o run-tests
	./run-tests
