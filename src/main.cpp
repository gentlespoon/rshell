#include "common.hpp"
#include "parse_commands.cc"

using namespace std;

  // Initialized for testing purpose: running code in Windows to emulate getuserinfo() gethostname()
  string user = "user";
  char host[128] = "localhost";

void init() {
  cout << "\n\nrShell [Version " << version << "]\n";
  // Unix get userinfo
  user = getlogin();
  gethostname(host, 128);
}

int newCmd() {
  // to distinguish with system shell I used "R$" instead of "$"
  cout << "\n[" << user << "@" << host << "] R$ ";
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);

  // override for testing
  if (newLine == "test1") {
    // bug exists in the commented testline; the boost/tokenizer class wont recognize \\\" in a quote.
    // but as long as there's no multiple \"s in a quote, it will function well.
    newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\ # inside a quote\"; echo \"Hello World\" #This is a real quote ";
    //newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\\" # inside a quote\"; echo \"Hello World\" #This is a real quote ";
  } else if (newLine == "test2") {
    newLine = "ls -a; echo hello velt && mkdir test || echo world; git status";
  }

  cmd cmd1(newLine);
  // cmd1.printline();
  cmd1.removeComment();
  cmd1.parseCmd(";");
  cmd1.parseCmd("&&");
  cmd1.parseCmd("||");
  cmd1.trimCmd();
  cmd1.printlist();

  // internal command handler;
  if (newLine == ""){

  } else if (newLine == "exit") {
    return -1; // use -1 as a exit signal
  } else if (newLine == "debug on") {
    DEV = true;
    cout << "Debug output is now turned on." << endl;
  } else if (newLine == "debug off") {
    DEV = false;
    cout << "Debug output is now turned off." << endl;
  } else {
    // TODO: execute the program here






















    return 0;
  }
}


int main(int argc, char *argv[]) {
  init();
  while (newCmd() != -1) { // loop until user enter "exit"
  };
  cout << "\n\n"; // kind of... clear the screen
  return 0;
}
