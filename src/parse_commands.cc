#ifndef _pc_cc_
#define _pc_cc_

#include "common.hpp"

using namespace std;
using namespace boost;

struct s_cmd {
    string exec;
    string file;
    string argv;
};

class cmd {
protected:
    string cmdLine;
    vector<string> cmdList;
public:

    string removeComment() {
        if (DEV) cout << "\n======= Start Remove Comment =======\n[Raw Line] " << cmdLine << endl << endl;
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




    vector<string> parse(string delim) {
        // if parsing the first time, initialize cmdList
        if (cmdList.size() == 0) { cmdList.push_back(cmdLine); }
        string trim_space;

        if (DEV) cout << "\n======= Start Parsing for " << delim << " =======\n";
        vector<string> p_cmdList;
        for (int i = 0; i < cmdList.size(); i++) {
            escaped_list_separator<char> els(/*escape*/"", /*seperator*/delim, /*quote*/"\"");
            tokenizer<escaped_list_separator<char> > tok(cmdList.at(i), els);
            for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end(); ++beg){
                if (*beg != "") {
                    trim_space = *beg;
                    trim(trim_space);
                    p_cmdList.push_back(trim_space);
                    cout << "<" << trim_space << ">" << endl;
                }
            }
        }
        cmdList = p_cmdList;
        if (DEV) cout << "\n======== End Parsing for " << delim << " ========\n";
        return p_cmdList;
    }




    cmd (string newLine) {
        cmdLine = newLine;
    }

    ~cmd () {
    }

    void printline() {
        cout << "\nThe command line is\n"<< this->cmdLine<< endl;
    }
    void printlist() {
        cout << "\nThe command list is\n";
        for (int i = 0; i < cmdList.size(); i++) {
          cout << cmdList.at(i) << endl;
        }
    }

};


#endif