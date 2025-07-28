/******************************************************************************

 Mini Shell - made in C++

    This is a super basic shell that runs in the terminal.
    It can run simple commands like "ls", "pwd", "echo", etc.
    It also supports "cd" to change directories, and "exit" to quit the shell.


*******************************************************************************/
#include <iostream>      
#include <unistd.h>     
#include <sys/wait.h>    
#include <vector>        
#include <sstream>       
#include <cstring>       
using namespace std;

// this function takes a line like "ls -l" and splits it into ["ls", "-l"]
vector<string> split(string line) {
    vector<string> words;
    istringstream stream(line); // use this to split by spaces
    string word;

    while (stream >> word) {
        words.push_back(word); // add each word to the vector
    }

    return words;
}

int main() {
    string line; // this will hold whatever the user types

    while (true) {
        // show a little prompt
        cout << "mini-shell> ";

        // get a full line of input from the user
        getline(cin, line);

        // if they just pressed Enter, ignore it
        if (line.empty()) continue;

        // if they type "exit", break the loop and quit
        if (line == "exit") break;

        // if they typed "cd" (change directory)
        if (line.substr(0, 2) == "cd") {
            string path = "."; // default to current folder

            // if they typed "cd something", get that folder name
            if (line.length() > 3)
                path = line.substr(3);

            // try to change folder
            if (chdir(path.c_str()) != 0)
                perror("cd failed"); // show error if it didn’t work

            continue; // skip the rest of the loop
        }

        // split the line into words (command and arguments)
        vector<string> args = split(line);
        vector<char*> c_args;

        // turn the vector<string> into vector<char*> for execvp
        for (int i = 0; i < (int)args.size(); i++) {
            c_args.push_back(&args[i][0]); // convert string to char*
        }

        c_args.push_back(nullptr); // execvp needs this at the end

        // now fork the process (make a copy)
        pid_t pid = fork();

        if (pid == 0) {
            // child process: run the command
            if (execvp(c_args[0], c_args.data()) == -1) {
                perror("command failed"); // if command doesn’t work
            }
            exit(1); // exit if execvp fails
        } else if (pid > 0) {
            // parent process: wait for child to finish
            wait(nullptr);
        } else {
            // fork failed
            perror("fork failed");
        }
    }

    cout << "Bye!\n";
    return 0;
}
