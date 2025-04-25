#include <iostream>
#include <cstring>

#include "Hash.h"

/*
 * name:      Hash
 * purpose:   This function will initialize the size of hash_table 
 * arguments: none
 * returns:   none
 * effects:   hash_table size will be initialized
 */
Hash::Hash() {
    currentTableSize = INITIAL_TABLE_SIZE;
    numItemsInTable = 0;
    hash_table.resize(currentTableSize);
}

/*
 * name:      ~Hash
 * purpose:   free allocated heap memory
 * arguments: none
 * returns:   none
 * effects:   Free heap memory created of Line pointer   
 */
Hash::~Hash() {
    for (size_t i = 0; i < lineStorage.size(); i++) {
        delete lineStorage[i];
    }
}

/*
 * name:      run()
 * purpose:   Reads commands from standard input and executes the appropriate 
 *            action based on the command.
 * arguments: - directory: the path to the root directory to be traversed.
 *            - fileName: the initial output file where search results 
 *                  are written.
 * returns:   none
 * effects:   Runs the full program loop until the quit command is given. 
 *            Opens and closes output files as needed and prints results to the 
 *            current output file.
 */
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

/*
 * name:      open_or_die
 * purpose:   Attempts to open the specified file stream. Exits the program 
 *            with an error message if the file cannot be opened.
 * arguments: 
 *            - stream: a reference to an input or output file stream
 *            - file_name: the name of the file to open.
 * returns:   None
 * effects:   Opens the file if possible; otherwise, prints an error message 
 *            and terminates the program.
 */
template<typename streamtype>
void Hash::open_or_die(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        cerr << "Error: could not open file " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}

/*
 * name:      searchWord
 * purpose:   Searches for a word in the hash table and writes the matching 
 *            lines to the output file.
 * arguments: - is_sen: a boolean indicating whether the search is 
 *                      case-sensitive (true) or case-insensitive (false).
 *            - word: the word to search for in the hash table.
 *            - output: the output file to write the results to.
 * returns:   None
 * effects:   Writes the search results to the output stream. If the word is 
 *            not found, outputs a corresponding "Not Found" message.
 */
void Hash::searchWord(bool is_sen, string word, ostream &output) {
    // true - sensitiev / false - insensitive
    string lower = lowerChar(word);
    int index = createKeyIndex(lower);

    while (!hash_table[index].empty() && 
                hash_table[index][0].insensitive_word != lower) {
        index = (index + 1) % hash_table.size();
    }

    if (hash_table[index].empty()) {
        if (is_sen) {
            output << word 
            << " Not Found. Try with @insensitive or @i." << endl;
        } else {
            output << word << " Not Found." << endl;
        }
    } else {
        if (is_sen) { 
            for (size_t i = 0; i < hash_table[index].size(); i++) {
                cout << hash_table[index].size() << endl;

                if (word.compare(hash_table[index][i].word) == 0) {
                    string line = "";
                    for (Line* l : hash_table[index][i].line) {
                        line = l->dir + ":" + to_string(l->line_num) 
                                + ": " + l->content + "\n";
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
                    line = l->dir + ":" + to_string(l->line_num) + ": " 
                                + l->content + "\n";
                    lineToPrint.insert(line);
                }
            }
            printToFile(output);
            lineToPrint.clear();
        }
    }
}

/*
 * name:      printToFile
 * purpose:   Outputs all stored lines from the lineToPrint unordered_set to 
 *            the provided output file.
 * arguments: - output: the output file to write the results to.
 * returns:   None
 * effects:   Writes the search results to the output stream. If the word is 
 *            not found, outputs a corresponding "Not Found" message.
 */
void Hash::printToFile(ostream &output) {
    for (string s : lineToPrint) {
        output << s;
    }
}

/*
 * name:      traverseDirectory
 * purpose:   Performs a breadth-first traversal (BFS) of the given directory 
 *            and processes all files by reading their lines and extracting 
 *            words.
 * arguments: - directory: the root directory path to start traversal from.
 * returns:   None
 * effects:   Dynamically creates an FSTree to represent the directory 
 *            structure, reads all files line by line, and passes each line 
 *            for word processing. Frees the FSTree memory after traversal.
 */
void Hash::traverseDirectory(string directory) {
    FSTree *fstree = new FSTree(directory);
    queue<pair<DirNode*, string>> dirQueue;
    dirQueue.push({fstree->getRoot(), fstree->getRoot()->getName() + "/"});
    while (not dirQueue.empty()) {
        DirNode *curr = dirQueue.front().first;
        string topString = dirQueue.front().second;
        dirQueue.pop();
        for (int i = 0; i < curr->numFiles(); i++) {
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

/*
 * name:      createLine
 * purpose:   Dynamically allocates and initializes a Line struct with the 
 *            provided information, and stores it for future memory management.
 * arguments: 
 *            - dir: the file path where the line was found
 *            - line_num: the line number within the file
 *            - content: the actual text content of the line
 * returns:   A pointer to the newly created Line object
 * effects:   Allocates memory on the heap for a Line and stores the pointer 
 *            in a vector for later cleanup.
 */
Line* Hash::createLine(string dir, int line_num, string content) {
    // Allocate memory for the new line
    Line* newLine = new Line();
    newLine->dir = dir;
    newLine->line_num = line_num;
    newLine->content = content;
    
    lineStorage.push_back(newLine);
    return newLine; // Return the newly created line
}

/*
 * name:      createEntry
 * purpose:   Constructs and returns an Entry struct that stores the original 
 *            word, its case-insensitive version, and the line pointer where 
 *            the word is located.
 * arguments: - word : the original word extracted from a line of text
 * returns:   An initialized Entry struct
 * effects:   Increments the total number of entries stored in the hash table
 */
Entry Hash::createEntry(string word) {
    Entry newEntry = Entry();
    newEntry.word = word;
    newEntry.insensitive_word = lowerChar(word);
    numItemsInTable++;
    return newEntry;
}

/*
 * name:      stripNonAlphaNum
 * purpose:   called stripNonAlhpaNumHelper to remove leading and trailing 
 *            non-alphanumeric characters
 * arguments: - word : the original word extracted from a line of text
 * returns:   A new string with non-alphanumeric characters removed from 
 *            both ends
 * effects:   None.
 */
string Hash::stripNonAlphaNum(string input) {
    return stripNonAlhpaNumHelper(input);
}

/*
 * name:      stripNonAlhpaNumHelper
 * purpose:   Removes leading and trailing non-alphanumeric characters from 
 *            the given string.
 * arguments: - word : reference to the string to be stripped
 * returns:   The modified string with surrounding non-alphanumeric characters 
 *            removed
 * effects:   None.
 */
string Hash::stripNonAlhpaNumHelper(string &input) {
    while (not input.empty() and not isalnum(input[0])) {
        input.erase(0, 1);
    }

    while (not input.empty() and not isalnum(input[input.length() - 1])) {
        input.erase(input.length() - 1, 1);
    }   
    return input;
}

/*
 * name:      readLines()
 * purpose:   read lines in the input file, create new line pointer, and pass
 *            the line to readWord to read words in the line
 * arguments: input file string
 * returns:   none
 * effects:   create new line pointer for each line in the file, then read
 *            every word in the line. 
 */
void Hash::readLines(string inputFile) {
    string line;
    ifstream infile(inputFile);
    int line_num = 0;
    while (getline(infile, line)) {
        line_num++;
        Line* l = createLine(inputFile, line_num, line);
        readWord(l);
    }
    
}

/*
 * name:      readWord()
 * purpose:   read the words in the line, pass the word and line to pushWord()
 *            to push the word and the line.
 * arguments: the line pointer containing this word
 * returns:   none
 * effects:   potentially create new word Entry, and push the line to the back
 *            of the lines list 
 */
void Hash::readWord(Line *line) {
    istringstream iss(line->content);
    string word;
    while (iss >> word) { 
        pushWord(word, line);
    }
}

/*
 * name:      expand()
 * purpose:   expand the hash table by doubling the size
 * arguments: none
 * returns:   none
 * effects:   doubling the size of hash table, copy all elements, and re-index
 *            all of them in the new hash table. 
 */
void Hash::expand() {
    int newSize = currentTableSize * 2;
    vector<vector<Entry>> newTable(newSize);  // new, bigger table

    for (const vector<Entry> &bucket : hash_table) {
        for (const Entry &entry : bucket) {
            int newIndex = hash<string>()(entry.insensitive_word) % newSize;

            while (not newTable[newIndex].empty() && 
                    newTable[newIndex][0].insensitive_word 
                        != entry.insensitive_word) {
                newIndex = (newIndex + 1) % newSize;
            }
            newTable[newIndex].push_back(entry);
        }
    }

    hash_table = newTable;
    currentTableSize = newSize;   // update size tracker
}

/*
 * name:      lowerChar()
 * purpose:   convert the word to lower form
 * arguments: the string to convert
 * returns:   the converted word
 * effects:   convert the passed in word to lower letters
 */
string Hash::lowerChar(string toConvert) {
    string strippedWd = stripNonAlphaNum(toConvert);
    for (char &ch : strippedWd) {
        ch = tolower(ch);
    }
    return strippedWd;
}

/*
 * name:      pushWord()
 * purpose:   find whether to push the word into the table or not. If new word,
 *            create new Entry, push at back of the index, and push line.
 *            If not new word, push the line to the back of the existing
 *            entry.
 * arguments: the string of passed word, and the line pointer to the passed
 *            line
 * returns:   void
 * effects:   if we encounter the new word, create a new entry and push the
 *            entry to the hash table, put the line in the vector
 */
void Hash::pushWord(string passed_word, Line *passed_line) {
    // find hash index, call createKeyIndex(word)
    string lower_word = lowerChar(passed_word);
    string stripped_word = stripNonAlphaNum(passed_word);
    
    if (numItemsInTable * 1.0 / currentTableSize >= 0.75) {
        expand();
    }

    int index = createKeyIndex(lower_word);

    // check if word exist, apply linear probing
    while (!hash_table[index].empty()) {
        bool same_insensitive = false;
        size_t entry_vector = hash_table[index].size();
        for (size_t i = 0; i < entry_vector; i++) {
            // if we find the sensitive word, return entry
            if (stripped_word.compare(hash_table[index][i].word) == 0) {
                hash_table[index][i].line.insert(passed_line);
                return;
            } 
            else if (lower_word.compare(hash_table[index][i].insensitive_word) 
                == 0) {
                same_insensitive = true;
            }
        }
        if (same_insensitive) {
            // create new entry and push at back of current entry vector
            pushWordHelper(stripped_word, passed_line, index);  
            return;
        }
        index = (index + 1) % hash_table.size();
    }
    // If not found in probing, create new entry
    pushWordHelper(stripped_word, passed_line, index);
}

/*
 * name:      pushWordHelper()
 * purpose:   push the new word to the back of the specific Entry vector in
 *            the hash table
 * arguments: the string of stripped word, the line pointer, and the index
 *            of the Entry vector to push the word
 * returns:   void
 * effects:   push the word to the back of the Entry vector in the hash table
 */
void Hash::pushWordHelper(string stripped, Line *line, int index) {
    // If not found in probing, create new entry
    Entry newEntry = createEntry(stripped);

    // push word to hash_table
    newEntry.line.insert(line);
    hash_table[index].push_back(newEntry);
}

/*
 * name:      createKeyIndex()
 * purpose:   get the index of a specific word in hash table
 * arguments: the word string
 * returns:   the integer indicating the word's index in hash table
 * effects:   compute the index of the word
 */
int Hash::createKeyIndex(string word) {
    size_t index = hash<string>()(word) % hash_table.size();
    return index;
}
