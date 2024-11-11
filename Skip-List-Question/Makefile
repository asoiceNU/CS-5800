CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Define the object files
OBJECTS = main.o SkipList.o SkipListNode.o

# Define the executable name
TARGET = skiplist

# Default rule to compile and link the program
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile main.cpp
main.o: main.cpp SkipList.hpp SkipListNode.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile SkipList.cpp
SkipList.o: SkipList.cpp SkipList.hpp SkipListNode.hpp
	$(CXX) $(CXXFLAGS) -c SkipList.cpp

# Compile SkipListNode.cpp
SkipListNode.o: SkipListNode.cpp SkipListNode.hpp
	$(CXX) $(CXXFLAGS) -c SkipListNode.cpp

# Clean up object files and executable
clean:
	rm -f $(OBJECTS) $(TARGET)
