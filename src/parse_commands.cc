#ifndef _pc_cc_
#define _pc_cc_

#include "common.hpp"

using namespace std;
using namespace boost;

struct s_cmd {
    string exec;
    string file;
    string argv;
    s_cmd() {
        exec = "";
        file = "";
        argv = "";
    }
    s_cmd(string a, string b, string c) {
        exec = a;
        file = b;
        argv = c;
    }
};

class cmd {
protected:
    string cmdLine;
    vector<s_cmd> cmdList;
public:

    string removeComment() {
        if (DEV) cout << "\n======= Start Remove Comment =======\n[Raw Line] " << cmdLine << endl;
        bool isInQuote = false;

        // Finished Quotation and Comments
        // ====== bug exist ======
        // For addresses that use \ instead of / (e.g. Windows), the program may mess up with escape characters
        for (unsigned index = 0; index < cmdLine.length(); index++) {
            
            // Dealing with "" quotes
            if (cmdLine.at(index) == '\"') {
                if (DEV) cout << "\" found at position " << index;
                if ((index == 0) || (cmdLine.at(index-1) != '\\')) {
                    if (DEV) cout << " This is a real quotation mark.\n";
                    isInQuote = !isInQuote;
                    if (DEV) cout << "[isInQuote] " << isInQuote << endl;
                }
                else {
                    if (isInQuote) {
                        if (DEV) cout << " This is a slashed quotation mark in a quote.\n";
                    }
                    else {
                        cout << "\n** WARNING: There might be a slashed quotation mark outside a quote!\n";
                    }
                    if (DEV) cout << "[isInQuote] " << isInQuote << endl;
                }
            }

            // Dealing with # comments
            if (cmdLine.at(index) == '#') {
                if (DEV) cout << "# found at position " << index;
                if (isInQuote) {
                    if (DEV) cout << ", however this # is in a Quote. Ignored.\n";
                }
                else {
                    if (DEV) cout << ", wow we found a comment!\n";
                    cmdLine = cmdLine.substr(0, index);
                    break;
                }
            }
        }
        // Finished Remove Comment
        if (isInQuote) { // if when the line ends we are still in a quote
            cout << "\n** WARNING: The line ends in a quote. \" expected.\n";
        }

        if (DEV) cout << "[Out Line] " << cmdLine << "\n======== End Remove Comment ========\n";
        return cmdLine;
    }




    vector<s_cmd> parseCmd(string delim) {
        // if parsing the first time, initialize cmdList
        if (cmdList.size() == 0) {
            s_cmd s_cmd1(";", "", cmdLine);
            cmdList.push_back(s_cmd1);
        }
        string trimmed;
        string file;
        string argv;
        unsigned space;
        if (DEV) cout << "\n======= Start Parsing for " << delim << " =======\n";
        vector<s_cmd> p_cmdList;
        for (int i = 0; i < cmdList.size(); i++) {
            string currexec = cmdList.at(i).exec;
            bool isHead = true;
            escaped_list_separator<char> els(/*escape*/"", /*separator*/delim, /*quote*/"\"");
            tokenizer<escaped_list_separator<char> > tok(cmdList.at(i).argv, els);
            for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end(); ++beg){
                if (*beg != "") {
                    trimmed = *beg;
                    trim(trimmed);
                    if (!isHead) {
                        currexec = delim;
                    }
                    space = trimmed.find(' ');
                    file = trimmed.substr(0, space);
                    argv = trimmed;
                    s_cmd s_cmd2(currexec, file, argv);
                    p_cmdList.push_back(s_cmd2);
                    if (DEV) cout << "[" << s_cmd2.exec << "] [" << s_cmd2.file << "] [" << s_cmd2.argv << "]\n";
                    isHead = false;
                }
            }
        }

        cmdList = p_cmdList;
        if (DEV) cout << "======== End Parsing for " << delim << " ========\n";
        return p_cmdList;
    }


    vector<s_cmd> trimCmd() {
        if (DEV) cout << "\n======= start trimCmd =======\n";
        for (int i = 0; i< cmdList.size(); i++) {
            if (cmdList.at(i).argv == cmdList.at(i).file) {
                cmdList.at(i).argv = "";
            }
            cmdList.at(i).argv = cmdList.at(i).argv.substr(cmdList.at(i).argv.find(' ')+1);
            if (DEV) printlist();
            if (DEV) cout << "======== end trimCmd ========\n";
        }
        return cmdList;
    }

    cmd (string newLine) {
        cmdLine = newLine;
    }

    ~cmd () {
    }

    void printline() {
        cout << this->cmdLine<< endl;
    }
    void printlist() {
        for (int i = 0; i < cmdList.size(); i++) {
            cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]\n";
        }
    }
    
    void generateExecCommand()
    {
        //flag to check if we run current command based on previous command
        bool previousStatus = true;
        bool runCurrentCommand = true;
        
        for (unsigned int i = 0; i < cmdList.size(); i++)
        {
            if (cmdList.at(i).exec == ";")
            {
                runCurrentCommand = true;
            }
            else if (cmdList.at(i).exec == "&&")
            {
                if (!previousStatus)
                    runCurrentCommand = false;
                else
                    runCurrentCommand = true;
            }
            else if (cmdList.at(i).exec == "||")
            {
                if (!previousStatus)
                    runCurrentCommand = true;
                else
                    runCurrentCommand = false;
            }
            if (runCurrentCommand)
            {   
                //copying vector commands into character array
                int arg1size = (cmdList.at(i).file).size();
                int arg2size = (cmdList.at(i).argv).size();
                char arg1[arg1size];
                char arg2[arg2size];
                strcpy(arg1, (cmdList.at(i).file).c_str());
                strcpy(arg2, (cmdList.at(i).argv).c_str());
                char* arg[] = {arg1, arg2, NULL};

                //set flag to see if command executed properly
                if (!exec(arg))
                    previousStatus = false;
                else
                    previousStatus = true;
            }
        }
    }
    //forking into parent and child processes in order to execute
    //passed in parse command
    bool exec(char* arg[])
    {
        int success;
        int status;
        pid_t c_pid, pid;
        c_pid = fork();
        if (c_pid == 0)
        {
            //child process running
            execvp(arg[0], arg);
            perror("exec failed");
            exit(1);
        }
        else if (c_pid > 0)
        {
            if ((pid = wait(&status)) < 0)
                perror("waiting");
            if (WIFEXITED(status))
                success = WEXITSTATUS(status);
            if (success == 0)
                return true;
            return false;
        }
        else
            perror("fork failed"); // is it.. fork failed and still return true?
        return true;
    }

};


#endif
