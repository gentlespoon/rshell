#include "common.hpp"
#include "parse_commands.cc"

using namespace std;

  // Initialized for testing purpose: running code in Windows to emulate getuserinfo() gethostname()
  string user = "user";
  char host[999] = "localhost";

void init() {
  cout << "\n\n rShell [Version " << version << "]\n\nEnter \"debug on\" to turn on debug output.\n\n";
  // Unix get userinfo
  user = getlogin();
  gethostname(host, 999);
}

int newCmd() {
  // to distinguish with system shell I used "R$" instead of "$"
  cout << "\n[" << user << "@" << host << "] R$ ";
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);
  if (newLine == ""){
    return 0;// if empty line
  } 
  // override for testing
  if (newLine == "test1") {
    // bug exists in the commented testline; the boost/tokenizer class wont recognize \\\" in a quote.
    // but as long as there's no multiple \"s in a quote, it will function well.
    newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\ # inside a quote\"; echo \"Hello World\" #This is a real quote ";
    //newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\\" # inside a quote\"; echo \"Hello World\" #This is a real quote ";
  } else if (newLine == "test2") {
    newLine = "ls -a; echo hello velt && mkdir test || echo world; git status; exit; ls -l";
  }

  cmd cmd1(newLine);
  cmd1.removeComment();
  cmd1.parseCmd(";");
  cmd1.parseCmd("&&");
  cmd1.parseCmd("||");
  cmd1.trimCmd();
  cmd1.generateExecCommand();
  return 0;
}


int main(int argc, char *argv[]) {
  init();
  while (newCmd() != -1) { // loop until user enter "exit"
  };
  cout << "\n\n"; // kind of... clear the screen
  return 0;
}
