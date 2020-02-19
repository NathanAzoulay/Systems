# makefile for hexdump program
# g++ compiler
CXX=g++
# flags
CXXFLAGS=-std=c++11 -g
# target with output of dbx executable
dbx: dbx.cpp
	$(CXX) $(CXXFLAGS) -o dbx dbx.cpp

# call clean to remove executable
clean:
	rm dbx