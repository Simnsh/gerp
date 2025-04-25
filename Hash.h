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
        template<typename streamtype>
        void open_or_die(streamtype &stream, string file_name);
        void searchWord(bool is_sen, string word, ostream &output);
        void printToFile(ostream &output);
        // void run(string directory, ostream &output);
        void run(string directory, string fileName);
        Line* createLine(string dir, int line_num, string content);
        Entry createEntry(string word);
        void readLines(string inputFile);
        void readWord(Line *sentence);
        void pushWord(string word, Line *line);
        void pushWordHelper(string stripped, Line *line, int index);
        int createKeyIndex(string word);
        string lowerChar(string toConvert);
        void traverseDirectory(string directory);
        string stripNonAlphaNum(string input);
        string stripNonAlhpaNumHelper(string &input);
        void expand();

        int currentTableSize;
        int numItemsInTable;

        vector<vector<Entry>> hash_table; 
        unordered_set<string> lineToPrint;


    private:
        size_t INITIAL_TABLE_SIZE = 10000;
        vector<Line*> lineStorage;
};

#endif
