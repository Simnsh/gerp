/*
 *  main.cpp
 *  Ian Goh, Vicky Zhu
 *  4/25/2025
 *
 *  CS 15 Project 4
 *
 *   This is the entry point of the GERP program. It takes a root directory and
 *  an output file as command-line arguments. The program recursively scans all
 *  text files within the given directory, indexes the words found, and then 
 *  allows users to query those words. Search results, including matching lines
 *  and file paths, are written to the specified output file.
 *
 */
#include "Hash.h"
#include "FSTree.h"

#include <istream>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        return EXIT_FAILURE;
    }

    string fileName = argv[2];
    Hash h;
    h.run(argv[1], fileName);
}

