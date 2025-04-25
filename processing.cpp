#include <queue>
#include <string>
#include <iostream>
#include <cctype>

#include "FSTree.h"
#include "DirNode.h"

using namespace std;

/*
 * name:      traverseDirectory()
 * purpose:   traverse the directory, print all paths to the files
 * arguments: the directory string
 * returns:   none
 * effects:   build the FSTree, do BFS, push each directory in queue, and
 *            print their directory, pop off the queue. Do this until empty
 */
void traverseDirectory(string directory) {
    FSTree *fstree = new FSTree(directory);
    queue<pair<DirNode*, string>> dirQueue;
    dirQueue.push({fstree->getRoot(), fstree->getRoot()->getName() + "/"});
    while (not dirQueue.empty()) {
        DirNode *curr = dirQueue.front().first;
        string topString = dirQueue.front().second;
        dirQueue.pop();
        for (int i = 0; i < curr->numFiles(); i++) {
            cout << topString << curr->getFile(i) << endl;
        }
        for (int i = 0; i < curr->numSubDirs(); i++) {
            dirQueue.push({curr->getSubDir(i), topString + 
                curr->getSubDir(i)->getName()+ "/"} );
        }
    }
    delete fstree;
}

/*
 * name:      stripNonAlhpaNumHelper()
 * purpose:   helper function to remove the nonalphanum chars in a string
 * arguments: the address to the original string
 * returns:   the string stripped off nonalphanum
 * effects:   strip the non-alphanum chars, change the original input string
 */
string stripNonAlhpaNumHelper(string &input) {
    while (not input.empty() and not isalnum(input[0])) {
        input.erase(0, 1);
    }

    while (not input.empty() and not isalnum(input[input.length() - 1])) {
        input.erase(input.length() - 1, 1);
    }   
    return input;
}

/*
 * name:      stripNonAlphaNum()
 * purpose:   remove the nonalphanum chars in a string
 * arguments: the original string
 * returns:   the string stripped off nonalphanum
 * effects:   strip the non-alphanum chars, change the original input string
 */
string stripNonAlphaNum(string input) {
    return stripNonAlhpaNumHelper(input);
}