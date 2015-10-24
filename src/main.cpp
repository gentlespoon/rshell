#include <cstdlib>
#include <boost/tokenizer.hpp>
#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <stddef.h>

bool DEV =true;
// global debug output control added.
// can be switched by "$ debug [on|off]" in runtime

using namespace std;
using namespace boost;


  // Initialized or testing purpose: running code in Windows to emulate getuserinfo() gethostname()
  string user = "asong011";
  char host[128] = "windows.local";

void init() {
  cout << "\n\nrShell [Version 2015.10.23]\n";
  // Unix get userinfo
  user = getlogin();
  gethostname(host, 128);
}

string removeComment(string newLine) {

  if (DEV) cout << "\n======= Start Remove Comment =======\n\n[Raw Line] " << newLine << endl << endl;
  bool isInQuote = false;

  // Finished Quotation and Comments
  // ====== bug exist ======
  // For addresses that use \ instead of / (e.g. Windows), the program may mess up with escape characters
  for (unsigned index = 0; index < newLine.length(); index++) {
    
    // Dealing with "" quotes
    if (newLine.at(index) == '\"') {
      if (DEV) cout << "\" found at position " << index;
      if ((index == 0) || (newLine.at(index-1) != '\\')) {
        if (DEV) cout << " This is a real quotation mark." << endl;
        isInQuote = !isInQuote;
        if (DEV) cout << "[isInQuote] " << isInQuote << endl;
      }
      else {
        if (isInQuote) {
          if (DEV) cout << " This is a slashed quotation mark in a quote." << endl;
        }
        else {
          cout << "\n** WARNING: There might be a slashed quotation mark outside a quote!" << endl;
        }
        if (DEV) cout << "[isInQuote] " << isInQuote << endl;
      }
    }

    // Dealing with # comments
    if (newLine.at(index) == '#') {
      if (DEV) cout << "# found at position " << index;
      if (isInQuote) {
        if (DEV) cout << ", however this # is in a Quote. Ignored.\n";
      }
      else {
        if (DEV) cout << ", wow we found a comment!";
        newLine = newLine.substr(0, index);
        break;
      }
    }
  }
  // Finished Remove Comment
  if (isInQuote) { // if when the line ends we are still in a quote
    cout << "\n** WARNING: The line ends in a quote. \" expected.\n";
  }

  if (DEV) cout << "\n[Final Line] " << newLine << "\n\n======== End Remove Comment ========\n";


  return newLine;
}


// based on boost library tokenizer.hpp
vector<string> splitLine(string newLine) {
  
  vector<string> cmds;

  //    This part is working
  // tokenizer<> tok(newLine);
  // for(tokenizer<>::iterator beg=tok.begin(); beg!=(tok.end());++beg){
  //    tokens.push_back(*beg);
  // }

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(" ");
  tokenizer tokens(newLine, sep);
  for (tokenizer::iterator tok_iter = tokens.begin();
       tok_iter != tokens.end(); ++tok_iter)
    cmds.push_back(*tok_iter);
  std::cout << "\n";
  return cmds;
}



int newCmd() {
  // to distinguish with system shell I used "R$" instead of "$"
  cout << "\n[" << user << "@" << host << "] R$ ";
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);

  newLine = removeComment(newLine);
  vector<string> tokens = splitLine(newLine);

  if (DEV) {
    cout << "\n[Tokenized commands]\n";
    for (int i = 0; i < tokens.size(); i++) {
      cout << tokens.at(i) << endl;
    }
  }


  if (newLine == "exit") {
    return -1; // use -1 as a exit signal
  }
  else if (newLine == "debug on") {
    DEV = true;
    cout << "Debug output is now turned on." << endl;
  }
  else if (newLine == "debug off") {
    DEV = false;













    cout << "Debug output is now turned off." << endl;
  }
  else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  init();
  while (newCmd() != -1) {
  };
  cout << "\n\n"; // kind of... clear the screen
  return 0;
}
