#include <cstdlib>
#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
// #include <windows.h>

bool DEV = true;
// global debug output control added.
// default DEV = true;
// can be switched by "$ debug [on|off]"

using namespace std;

  // Initialized or testing purpose: running code in Windows to emulate getuserinfo() gethostname()
  string user = "asong011";
  char host[128] = "windows.local";

void init() {
  cout << "\n\nrShell [Version 2015.10.23]\n";
  // Unix get userinfo
  // user = getlogin();
  // gethostname(host, 128);

}

int newCmd() {
  cout << "\n[" << user << "@" << host << "] R$ ";
  // to distinguish with system shell I used "R$" instead of "$"
  
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);
  if (DEV) cout << "\n========== Start Parsing ==========\n\n[Raw Command] " << newLine << endl;

  // debloat comments ** with bug: comments in "#".

  bool isInQuote = false;


  // first time find " second time find #, with bug - # in ""
/*
  unsigned index = 0;
  while (string::npos != (index = newLine.find("\"", index ))) {
    cout << "Find " << newLine.at(index) << " at: " << index << endl;
    // check if there exists a quote.
    if ((index == 0) || (newLine.at(index - 1) != '\\')) {
      isInQuote = !isInQuote;
      cout << "isInQuote State: " << isInQuote << endl;
    }
    else if (!isInQuote) {
      cout << "* \\\" exists before first \"\n";
    }
    else {
      cout << "\nThis one is in Quote\n";
    }
    index++;
  }
  if (isInQuote) {
    cout << "\n * isInQuote State = " << isInQuote << " , check your command.\n";
  }
  cout << endl;
  newLine = newLine.substr(0, newLine.find("#"));
  cout << "[Trimmed #] " << newLine << endl;
*/


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
          cout << "\nWARNING: There might be a slashed quotation mark outside a quote!" << endl;
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


  // Finished parsing line

  if (isInQuote) { // if when the line ends we are still in a quote
    cout << "\nWARNING: The line ends in a quote. \" expected.\n";
  }

  if (DEV) cout << "\n[Final Command] " << newLine << "\n\n=========== End Parsing ===========\n";

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

int main() {
  init();
  while (newCmd() != -1) {
  };
  cout << "\n\n"; // kinda clear the screen
  return 0;
}