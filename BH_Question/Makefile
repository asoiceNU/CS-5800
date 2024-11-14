# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Object files
OBJ = BH_Node.o BinomialHeap.o main.o

# Output executable
EXEC = binomial_heap

# Default target
all: $(EXEC)

# Rule for compiling the main program
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

# Rule for compiling BH_Node.cpp
BH_Node.o: BH_Node.cpp BH_Node.hpp
	$(CXX) $(CXXFLAGS) -c BH_Node.cpp

# Rule for compiling BinomialHeap.cpp
BinomialHeap.o: BinomialHeap.cpp BinomialHeap.hpp BH_Node.hpp
	$(CXX) $(CXXFLAGS) -c BinomialHeap.cpp

# Rule for compiling main.cpp
main.o: main.cpp BinomialHeap.hpp BH_Node.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(EXEC)

# Rebuild everything
rebuild: clean all

