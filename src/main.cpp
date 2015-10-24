#include <cstdlib>
#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
// #include <windows.h>

using namespace std;

  
  // Initialized or testing purpose: running code in Windows to emulate getuserinfo() gethostname()
  string user = "asong011";
  char host[128] = "windows.local";

void init() {
  cout << "\n\nrShell [Version 2015.10.23]\n";
  // Unix get userinfo
  user = getlogin();
  gethostname(host, 128);

}

int newCmd() {
  // vector<pair cmd> cmdList;
  cout << "\n[" << user << "@" << host << "] R$ ";
  // to distinguish with system shell I used "R$" instead of "$"
  
  string newLine;
  getline(cin, newLine);
  cout << 
"\n========== Start Parsing ==========\n\n[Raw Input] " << newLine << endl;

  // debloat comments ** with bug: comments in "#".

  bool isInQuote = false;


  // first time find " second time find #, with bug - # in ""
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


  cout << "\n========== End Parsing =========\n\n[Final Command] " << endl << newLine << endl;

  if (newLine == "exit") {
    return -1; // use -1 as a exit signal
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