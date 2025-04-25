#include <iostream>
#include <cstring>

#include "Hash.h"

Hash::Hash() {
    currentTableSize = INITIAL_TABLE_SIZE;
    numItemsInTable = 0;
    hash_table.resize(currentTableSize);
}

Hash::~Hash() {
    for (size_t i = 0; i < lineStorage.size(); i++) {
        delete lineStorage[i];
    }
}

void Hash::run(string directory, string fileName) {
    traverseDirectory(directory);
    ofstream output;
    open_or_die(output, fileName);
    
    
    string query, word, newFile;
    cout << "Query? ";
    cin >> query;
    while (not cin.fail()) {
        if ((query == "@q") or (query == "@quit")) {
            cout << "Goodbye! Thank you and have a nice day." << endl;
            return;
        } else if ((query == "@i") or (query == "insensitive")) {
            cin >> word;
            searchWord(false, word, output);

            // call searchword(bool sensitive, string word)
        } else if (query == "@f") {
            cin >> newFile;
            output.close();
            open_or_die(output, newFile);
        } else {
            searchWord(true, query, output);

            // search word
        }
        cout << "Query? ";
        cin >> query;
    }
    output.close();
}

// void Hash::run(string directory, ostream &output) {
//     traverseDirectory(directory);
//     ostream* initialOutput = &output;
    
    
//     string query, word, newFile;
//     cout << "Query? ";
//     cin >> query;
//     while (not cin.fail()) {
//         if ((query == "@q") or (query == "@quit")) {
//             cout << "Goodbye! Thank you and have a nice day." << endl;
//             return;
//         } else if ((query == "@i") or (query == "insensitive")) {
//             cin >> word;
//             searchWord(false, word, output);

//             // call searchword(bool sensitive, string word)
//         } else if (query == "@f") {
//             cin >> newFile;
//             output.close();
//             ofstream new_outstream;
//             pen_or_die(new_outstream, newFile);
//             output = new_outstream;

//             if (new_outstream.is_open()) {
//                 new_outstream.close();
//             }
//             open_or_die(new_outstream, newFile);
//             initialOutput = &new_outstream;
            
//             new_outstream.close();
//             // close old file

//             // open new file
//         } else {
//             searchWord(true, query, output);

//             // search word
//         }
//         cout << "Query? ";
//         cin >> query;
//     }

// }

template<typename streamtype>
void Hash::open_or_die(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        cerr << "Error: could not open file " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}

void Hash::searchWord(bool is_sen, string word, ostream &output) {
    // true - sensitiev / false - insensitive
    string lower = lowerChar(word);
    int index = createKeyIndex(lower);

    while (!hash_table[index].empty() && hash_table[index][0].insensitive_word != lower) {
        index = (index + 1) % hash_table.size();
    }

    if (hash_table[index].empty()) {
        if (is_sen) {
            output << word << " Not Found. Try with @insensitive or @i." << endl;
        } else {
            output << word << " Not Found." << endl;
        }
    } else {
        if (is_sen) { // word is sen, search within vector<Entry> and output lines
            for (size_t i = 0; i < hash_table[index].size(); i++) {
                cout << hash_table[index].size() << endl;

                if (word.compare(hash_table[index][i].word) == 0) {
                    string line = "";
                    for (Line* l : hash_table[index][i].line) {
                        line = l->dir + ":" + to_string(l->line_num) + ": " + l->content + "\n";
                        lineToPrint.insert(line);
                    }
                    printToFile(output);
                    lineToPrint.clear();
                    // return;
                }
            }
        } else { // word is insen
            for (size_t i = 0; i < hash_table[index].size(); i++) {
                string line= "";
                for (Line* l : hash_table[index][i].line) {
                    line = l->dir + ":" + to_string(l->line_num) + ": " + l->content + "\n";
                    lineToPrint.insert(line);
                }
            }
            printToFile(output);
            lineToPrint.clear();
        }
    }

    // delete later! debug purpose
    // for (Entry w : hash_table[index]) {
    //     cout << "Current sensitive word : " << w.word << endl;
    //     cout << "Current insensitive word : " << w.insensitive_word << endl;
    //     cout << "Index is : " << index << endl;
    //     for (Line* l : w.line) {
    //         cout << "Line: " << l->line_num << ": " << l->dir << "/" << l->content << endl;
    //     }
    // }
}


void Hash::printToFile(ostream &output) {
    for (string s : lineToPrint) {
        output << s;
    }
}

void Hash::traverseDirectory(string directory) {
    FSTree *fstree = new FSTree(directory);
    queue<pair<DirNode*, string>> dirQueue;
    dirQueue.push({fstree->getRoot(), fstree->getRoot()->getName() + "/"});
    while (not dirQueue.empty()) {
        DirNode *curr = dirQueue.front().first;
        string topString = dirQueue.front().second;
        dirQueue.pop();
        for (int i = 0; i < curr->numFiles(); i++) {
            // cout << topString << curr->getFile(i) << endl;
            string fileName = topString + curr->getFile(i);
            readLines(fileName);
        }
        for (int i = 0; i < curr->numSubDirs(); i++) {
            dirQueue.push({curr->getSubDir(i), topString + 
                curr->getSubDir(i)->getName()+ "/"} );
        }
    }
    delete fstree;
}

Line* Hash::createLine(string dir, int line_num, string content) {
    // Allocate memory for the new line
    Line* newLine = new Line();
    newLine->dir = dir;
    newLine->line_num = line_num;
    newLine->content = content;
    
    lineStorage.push_back(newLine);
    return newLine; // Return the newly created line
}

Entry Hash::createEntry(string word) {
    Entry newEntry = Entry();
    newEntry.word = word;
    newEntry.insensitive_word = lowerChar(word);
    numItemsInTable++;
    return newEntry;
}

string Hash::stripNonAlphaNum(string input) {
    return stripNonAlhpaNumHelper(input);
}

string Hash::stripNonAlhpaNumHelper(string &input) {
    while (not input.empty() and not isalnum(input[0])) {
        input.erase(0, 1);
    }

    while (not input.empty() and not isalnum(input[input.length() - 1])) {
        input.erase(input.length() - 1, 1);
    }   
    return input;
}

void Hash::readLines(string inputFile) {
    string line;
    ifstream infile(inputFile);
    int line_num = 0;
    while (getline(infile, line)) {
        line_num++;
        Line* l = createLine(inputFile, line_num, line);
        readWord(l);
        // cout << "Line content:" << l->content << endl;
        // cout << "Line dir: " << l->dir << endl;
        // cout << "Line number:" << l->line_num << endl;
    }
    
}

void Hash::readWord(Line *line) {
    istringstream iss(line->content);
    string word;
    while (iss >> word) { 
        pushWord(word, line);
        // cout << "currWord: " << currWord.word << endl;
        // cout << "insensitive word: " << currWord.insensitive_word << endl;
        // for (Line* wd : currWord.line) {
        //     cout << "Line num: " << wd->line_num << endl;
        //     cout << "Line content: " << wd->content << endl;
        //     cout << "Line directory: " << wd->dir << endl;
        // }
    }
}

// void Hash::expand() {
//     vector<vector<Entry>> new_hash_table;
//     int newTableSize = currentTableSize*2;
//     new_hash_table.resize(newTableSize);
//     for (int i = 0; i < currentTableSize; i++) {
//         if (not hash_table[i].empty()) {
//             size_t index = hash<string>()(hash_table[i][0].insensitive_word) 
//                 % newTableSize;
//             vector<Entry> element = hash_table[i];
//             new_hash_table[index] = element;
//         }
//     }
//     currentTableSize = newTableSize;
//     hash_table = new_hash_table;
    
// }

void Hash::expand() {
    int newSize = currentTableSize * 2;
    vector<vector<Entry>> newTable(newSize);  // new, bigger table

    for (const vector<Entry> &bucket : hash_table) {
        for (const Entry &entry : bucket) {
            int newIndex = hash<string>()(entry.insensitive_word) % newSize;

            while (not newTable[newIndex].empty() && newTable[newIndex][0].insensitive_word != entry.insensitive_word) {
                newIndex = (newIndex + 1) % newSize;
            }
            newTable[newIndex].push_back(entry);
        }
    }

    hash_table = newTable;
    currentTableSize = newSize;   // update size tracker
}

string Hash::lowerChar(string toConvert) {
    string strippedWd = stripNonAlphaNum(toConvert);
    for (char &ch : strippedWd) {
        ch = tolower(ch);
    }
    return strippedWd;
}


void Hash::pushWord(string passed_word, Line *passed_line) {
    // find hash index, call createKeyIndex(word)
    string lower_word = lowerChar(passed_word);
    string stripped_word = stripNonAlphaNum(passed_word);
    
    if (numItemsInTable * 1.0 / currentTableSize >= 0.75) {
        expand();
    }

    int index = createKeyIndex(lower_word);


        // // Debug output: initial word info
        // cout << "\n--- Pushing word: " << passed_word << " ---" << endl;
        // cout << "Stripped word: " << stripped_word << " | Lowercase word: " << lower_word << endl;
        // cout << "Initial index: " << index << endl;

    // check if word exist, apply linear probing
    while (!hash_table[index].empty()) {
        // cout << "Checking index: " << index << " with " << hash_table[index].size() << " entries" << endl;


        bool same_insensitive = false;
        // vector<Entry> words = hash_table[index];
        size_t entry_vector = hash_table[index].size();
        for (size_t i = 0; i < entry_vector; i++) {
            // cout << "  Entry #" << i << ": word = " << hash_table[index][i].word
            //      << ", insensitive = " << hash_table[index][i].insensitive_word << endl;


            // if we find the sensitive word, return entry
            if (stripped_word.compare(hash_table[index][i].word) == 0) {
                // cout << "  Found exact sensitive match. Adding line and returning.\n";

                hash_table[index][i].line.insert(passed_line);
                return;
            } 
            else if (lower_word.compare(hash_table[index][i].insensitive_word) == 0) {
                // cout << "  Found matching insensitive group.\n";

                same_insensitive = true;

                // cout << "inside else if, word is : " << passed_word << endl;

                // // create new entry and push at back of current entry vector
                // pushWordHelper(stripped_word, passed_line, index);
                // return;
            }
        }
        

        if (same_insensitive) {
            // cout << "  Adding new variant '" << stripped_word << "' to existing insensitive group at index " << index << endl;

            // create new entry and push at back of current entry vector
            pushWordHelper(stripped_word, passed_line, index);
            
            // delete later, debug purpose
            // cout << "Entries after insertion at index " << index << ":\n";
            // for (const Entry& entry : hash_table[index]) {
            //     cout << "  - word: " << entry.word << ", insensitive: " << entry.insensitive_word << endl;
            // }

            return;
        }
        // for (size_t i = 0; i < entry_vector; i++) {
        //     if (lower_word.compare(hash_table[index][i].insensitive_word) == 0) {
        //         pushWordHelper(stripped_word, passed_line, index);
        //         return;  
        //     }
        // }

        // pushWordHelper(stripped_word, passed_line, index);
        // return;


        index = (index + 1) % hash_table.size();
    }



    // If not found in probing, create new entry
    pushWordHelper(stripped_word, passed_line, index);
}

void Hash::pushWordHelper(string stripped, Line *line, int index) {
    // If not found in probing, create new entry
    Entry newEntry = createEntry(stripped);

    // push word to hash_table
    newEntry.line.insert(line);
    hash_table[index].push_back(newEntry);

    // cout << "  >> pushWordHelper: inserting word '" << newEntry.word 
    //  << "', insensitive = '" << newEntry.insensitive_word 
    //  << "' at index " << index << endl;

    // for (Entry w : hash_table[index]) {
    //     cout << "Current sensitive word : " << w.word << endl;
    //     cout << "Current insensitive word : " << w.insensitive_word << endl;
    //     cout << "Index is : " << index << endl;
    // }
}

int Hash::createKeyIndex(string word) {
    size_t index = hash<string>()(word) % hash_table.size();
    return index;
}
