###
### Makefile for Gerp Project
###
### Author:  Ian Goh, Vicky Zhu

## 
## Build up your Makefile in a similar manner as for Zap. Feel free 
## to use that Makefile to guide you! Note, you do NOT need rules for
## FSTree.o and DirNode.o as those (pre-compiled) object files are 
## provided to you.
## 
## At the end, you can delete this comment!
## 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 
##
## Here is a special rule that removes all .o files besides the provided ones 
## (DirNode.o and FSTree.o), all temporary files (ending with ~), and 
## a.out produced by running unit_test. First, we find all .o files 
## that are not provided files, then we run rm -f on each found file 
## using the -exec find flag. Second, we delete the temporary files
## and a.out. @ is used to suppress stdout.
## 
## You do not need to modify or further comment this rule!
##

gerp: main.o FSTree.o DirNode.o Hash.o 
	${CXX} ${LDFLAGS} -o $@ $^

main.o: main.cpp FSTree.h Hash.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# processing.o: processing.cpp processing.h FSTree.h DirNode.h
# 	$(CXX) $(CXXFLAGS) -c processing.cpp

Hash.o: Hash.cpp Hash.h FSTree.h DirNode.h
	$(CXX) $(CXXFLAGS) -c Hash.cpp 

unit_test: unit_test_driver.o processing.o FSTree.o DirNode.o Hash.o
	$(CXX) $(CXXFLAGS) $^

clean:
	@find . -type f \( \
		-name '*.o' ! -name 'FSTree.o' ! -name 'DirNode.o' \
		\) -exec rm -f {} \;
	@rm -f *~ a.out

