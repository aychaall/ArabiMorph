CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = src/Root.cpp src/Node.cpp src/BinarySearchTree.cpp \
      src/hashtable.cpp src/core_engine.cpp main_cli.cpp

TARGET = morpho_engine

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
