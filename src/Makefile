CXX = g++
CXXFLAGS = -Wall -g

# Build the executable
all: main

# Link the object files into the executable
main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o

# Compile the source files into object files
main.o: main.cpp Board.hpp Pieces.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Clean up the directory
clean:
	rm -f *.o main
