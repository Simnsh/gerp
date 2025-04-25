#include "Hash.h"
#include "FSTree.h"

#include <istream>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

template<typename streamtype>
void open_or_die(streamtype &stream, string file_name);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        return EXIT_FAILURE;
    }
    // ofstream outstream;

    string fileName = argv[2];
    // open_or_die(outstream, argv[2]);
    Hash h;
    h.run(argv[1], fileName);
    
    // outstream.close();
}

// int main(int argc, char *argv[]) {
//     if (argc != 3) {
//         cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
//         return EXIT_FAILURE;
//     }
//     ofstream outstream;
//     open_or_die(outstream, argv[2]);
//     Hash h;
//     h.run(argv[1], outstream);
    
//     outstream.close();
// }

/*
 * Try to open the given file.
 * Abort on failure.
 * If the function returns, caller can assume stream is open.
 * By using a template, we gain modularity: this function will work
 * for ifstreams *and* ofstreams.
 */
template<typename streamtype>
void open_or_die(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        cerr << "Error: could not open file " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}
