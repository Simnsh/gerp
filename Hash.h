/*
 *  Hash.h
 *  Ian Zien Goh, Vicky Yifan Zhu
 *  Apr 25 2025
 *
 *  CS 15 Proj 4 Gerp
 *
 *  Interface of the Hash class. The Hash table includes two structs: Line
 *  and Entry. The Hash class handle functions as follows:
 *  (1) Parse the output stream user indicate;
 *  (2) Parse the directory user indicate;
 *  (3) Put the lines and words in the directory into a hash table. 
 *  (4) Ask for user's query of search word. Search the word in the hash
 *      table. 
 *  (5) If the word is found, write the lines number, directory, content that
 *      containing the word to the output stream. 
 *  (6) If the word is not found, write not found message to the output stream
 *  (7) The program supports those query:
        - Anystring (For sensitive word search)
        - @i Anystring / @insensitive Anystring (For insensitive word search)
        - @f newFile (Redirect the output to new file)
        - @q / @quit (quit the program)
 */

#ifndef _HASH_H_
#define _HASH_H_

#include <functional>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <istream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cctype>

#include "FSTree.h"
#include "DirNode.h"

using namespace std;

struct Line {
        string dir;
        int line_num;
        string content;
};

struct Entry {
        string word;
        string insensitive_word;
        unordered_set<Line*> line;
};

class Hash {
    public:
        Hash();
        ~Hash();
        void run(string directory, string fileName);

        string stripNonAlphaNum(string input);

        template<typename streamtype>
        void open_or_die(streamtype &stream, string file_name);
        void searchWord(bool is_sen, string word, ostream &output);
        void searchWordHelper(bool is_sen, 
                int index, string word, ostream &output);
        void printToFile(ostream &output);
        Line* createLine(string dir, int line_num, string content);
        Entry createEntry(string word);
        void readLines(string inputFile);
        void readWord(Line *sentence);
        void pushWord(string word, Line *line);
        void pushWordHelper(string stripped, Line *line, int index);
        int createKeyIndex(string word);
        string lowerChar(string toConvert);
        void traverseDirectory(string directory);
        string stripNonAlhpaNumHelper(string &input);
        void expand();

        int currentTableSize;
        int numItemsInTable;

        vector<vector<Entry>> hash_table; 
        unordered_set<string> lineToPrint;

        size_t INITIAL_TABLE_SIZE = 10000;
        vector<Line*> lineStorage;

    private:

};

#endif
