/*
 *  unit_tests.h
 *  Ian Goh, Vicky Zhu
 *  4/25/2025
 *
 *  CS 15 Project 4
 * 
 *  This file is to test out functinality of each mathods in Hash.cpp
 */
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>


#include "processing.h"
#include "Hash.h"

/*
 * traverseDirectory_test1
 * Verify that traverseDirectory correctly print out all the path from tinyData
 */
void traverseDirectory_test1() {
    traverseDirectory("/comp/15/files/proj-gerp-test-dirs/tinyData");
}

/*
 * traverseDirectory_test2
 * Verify that traverseDirectory correctly print out all the path from 
 * smallGutenberg
 */
void traverseDirectory_test2() {
    traverseDirectory("/comp/15/files/proj-gerp-test-dirs/smallGutenberg");
}

/*
 * isAlpha_test1
 * Verify that stripNonAlphaNum correctly strip all the non alphabet or num char
 * from the front and the back of input string
 */
void isAlpha_test1() {
    string ori = stripNonAlphaNum("@##!!#!@!#COMP-15!!!!!!!");
    cout << ori << endl;
    assert(ori.compare("COMP-15") == 0);
}

/*
 * isAlphaHelper_test
 * Verify the non-alphanum before and after COMP-15 is removed
 */
void isAlphaHelper_test() {
    string input = "@##!!#!@!#COMP-15!!!!!!!";
    string ori = stripNonAlhpaNumHelper(input);
    cout << ori << endl;

}

/*
 * isAlphaHelper_test2
 * Verify the non-alphanum after Football is removed
 */
void isAlphaHelper_test2() {
    string input = "Football!!!!!!!";
    string ori = stripNonAlhpaNumHelper(input);
    cout << ori << endl;
}

void isAlphaHelper_test3() {
    string input = ")(*&^%$%^&*(*&^%^&^)(*&^%^&*(*&^%^&";
    string ori = stripNonAlhpaNumHelper(input);
    cout << "Result: " << ori << endl;
}

// use diffchecker visually check the line print out to terminal
// cout line from readLines in Hash.cpp, but cout will be remove by the time
// this project is submitted
void readLines_test1() {
    Hash hash1;
    hash1.readLines("/comp/15/files/proj-gerp-test-dirs/tinyData/test.txt");

}

void readLines_test2() {
    Hash hash1;
    hash1.readLines("/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                        "www.gutenberg.lib.md.us/1/0/0/4/10040/10040.txt");
}

/*
 * readWord_test1
 * Verify if entry print the same line for each word and all related info
 * about the line correctly
 */
void readWord_test1() {
    Line *line1 = new Line;
    line1->content = "at filename 24689 would be found at:";
    line1->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                    "www.gutenberg.lib.md.us/1/0/0/4/10040/10040.txt";
    line1->line_num = 1047;

    Hash hash1;
    hash1.readWord(line1);

    delete line1;
}

/*
 * readWord_test2
 * Verify if unordered_set in Entry store different lines when there is a word
 * that appear in the same textfile
 */
void readWord_test2() {
    Line *line1 = new Line;
    line1->content = "at filename 24689 would be found at 24689:";
    line1->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                    "www.gutenberg.lib.md.us/1/0/0/4/10040/10040.txt";
    line1->line_num = 1047;

    Line *line2 = new Line;
    line2->content = "look LOOK at me AT jcc 5th floor!";
    line2->dir = "dir_2";
    line2->line_num = 1048;

    Hash hash1;
    hash1.readWord(line1);
    hash1.readWord(line2);

    delete line1;
    delete line2;
}

void expand_test1() {
    Line *line1 = new Line;
    line1->content = "at filename 24689 would be found at 24689:";
    line1->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg"
                    "/www.gutenberg.lib.md.us/1/0/0/4/10040/10040.txt";
    line1->line_num = 1047;  

    Line *line2 = new Line;
    line2->content = "look ahh lol LOOK at me AT jcc 5th floor!";
    line2->dir = "dir_2";
    line2->line_num = 1048;
    
    Hash hash1;
    hash1.readWord(line1);
    hash1.readWord(line2);
    
    cout << "current table size: " << hash1.currentTableSize << endl;
    cout << "Num items: " << hash1.numItemsInTable << endl;
    cout << "Load factor: " 
            << hash1.numItemsInTable * 1.0 / hash1.currentTableSize << endl;
    assert(hash1.currentTableSize == 10000);

    delete line1;
    delete line2;
}

void expand_test2() {
    Line *line1 = new Line;
    line1->content = "at filename 24689 would be found at 24689:";
    line1->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg"
                "/www.gutenberg.lib.md.us/1/0/0/4/10040/10040.txt";
    line1->line_num = 1047;  

    Line *line2 = new Line;
    line2->content = "look ahh lol LOOK at me AT jcc 5th floor!";
    line2->dir = "dir_2";
    line2->line_num = 1048;
    
    Hash hash1;
    hash1.readWord(line1);
    hash1.readWord(line2);
    
    for (vector<Entry> l : hash1.hash_table) {
        for (Entry e: l) {
            cout << "Current word: " << e.word << endl;
        }
    }

    delete line1;
    delete line2;
}

void expand_test3() {
    Line *line1 = new Line;
    line1->content = "at filename 24689 would be found at 24689:";
    line1->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/" 
                 "www.gutenberg.lib.md.us/1/0/0/4/10040/10040.txt";
    line1->line_num = 1047;  

    Line *line2 = new Line;
    line2->content = "look ahh lol LOOK at me AT jcc 5th floor!";
    line2->dir = "dir_2";
    line2->line_num = 1048;
    
    Hash hash1;
    hash1.readWord(line1);

    cout << "Hash_table Iteration 1:" << endl;
    for (size_t i = 0; i < hash1.hash_table.size(); i++) {
        for (size_t j = 0; j < hash1.hash_table[i].size(); j++) {
            cout << "Word: " << hash1.hash_table[i][j].word << endl;
            cout << "Insensitive Word: " 
                << hash1.hash_table[i][j].insensitive_word << endl;
            cout << "Index: " << i << endl;
            cout << "inner Line index: " << j << endl;
            unordered_set<Line*> currLine = hash1.hash_table[i][j].line;
            cout << "size of unordedred set: " 
                << hash1.hash_table[i][j].line.size() << endl;

            int currIndex = 0;
            for (Line* val : currLine) {
                cout << "Current index in unordered_set: " << currIndex << endl;
                cout << "Current line content: " << val->content << endl;
                cout << "Current line dir: " << val->dir << endl;
                cout << "Current line number: " << val->line_num << endl;
                currIndex++;
            }
        }
    }

    hash1.readWord(line2);

    cout << "\n \n Hash_table Iteration 2:" << endl;
    for (size_t i = 0; i < hash1.hash_table.size(); i++) {
        for (size_t j = 0; j < hash1.hash_table[i].size(); j++) {
            cout << "Word: " << hash1.hash_table[i][j].word << endl;
            cout << "Insensitive Word: " 
                << hash1.hash_table[i][j].insensitive_word << endl;
            cout << "Hash_table Index: " << i << endl;
            cout << "inner Line index: " << j << endl;
            unordered_set<Line*> currLine = hash1.hash_table[i][j].line;
            cout << "size of unordedred set: " 
                << hash1.hash_table[i][j].line.size() << endl;

            int currIndex = 0;
            for (Line* val : currLine) {
                cout << "Current index in unordered_set: " << currIndex << endl;
                cout << "Current line content: " << val->content << endl;
                cout << "Current line dir: " << val->dir << endl;
                cout << "Current line number: " << val->line_num << endl;
                currIndex++;
            }
        }
    }
    
    cout << endl << endl;
    cout << "current table size: " << hash1.currentTableSize << endl;
    cout << "Num items: " << hash1.numItemsInTable << endl;
    cout << "Load factor: " 
         << hash1.numItemsInTable * 1.0 / hash1.currentTableSize << endl;

    delete line1;
    delete line2;
}

void push_word_test() {
    Hash h;

    Line *line1 = new Line;
    line1->content = "stripped from the waist upwards, a youth of about "
                    "fifteen years of age,";
    line1->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                "www.gutenberg.lib.md.us/5/9/0/5904/5904.txt";
    line1->line_num = 70;  
    
    h.pushWord("age", line1);

    Line *line2 = new Line;
    line2->content = "Age after age, the barren and meaningless process";
    line2->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                "www.gutenberg.lib.md.us/7/5/5/7556/7556.txt";
    line2->line_num = 243;  

    h.pushWord("Age", line2);


    Line *line3 = new Line;
    line3->content = "Age after age, the barren and meaningless process";
    line3->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                "www.gutenberg.lib.md.us/7/5/5/7556/7556.txt";
    line3->line_num = 243; 
    h.pushWord("age", line3);

    Line *line4 = new Line;
    line4->content = "THE GILDED AGE";
    line4->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                "www.gutenberg.lib.md.us/7/5/5/7556/7556.txt";
    line4->line_num = 438; 
    h.pushWord("AGE", line4);

    Line *line5 = new Line;
    line5->content = "Age, that cruel and unavoidable disease";
    line5->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/"
                "www.gutenberg.lib.md.us/7/5/3/7538/7538.txt";
    line5->line_num = 59; 
    h.pushWord("Age", line5);

    Line *line6 = new Line;
    line6->content = "Happy Age when one's inexperience is";
    line6->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/" 
                 "www.gutenberg.lib.md.us/7/5/3/7538/7538.txt";
    line6->line_num = 204; 
    h.pushWord("Age", line6);

    Line *line7 = new Line;
    line7->content = "He used to ask us wot was going to happen to 'im in his" 
                     " old age, and when";
    line7->dir = "/comp/15/files/proj-gerp-test-dirs/smallGutenberg/" 
                 "www.gutenberg.lib.md.us/1/1/1/8/11181/11181.txt";
    line7->line_num = 56; 
    h.pushWord("age", line7);

    h.searchWord(true, "AGE", cout);

    delete line1;
    delete line2;
    delete line3;
    delete line4;
    delete line5;
    delete line6;
    delete line7;
}

