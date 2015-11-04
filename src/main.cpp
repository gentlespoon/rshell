#include "common.hpp"
#include "parse_commands.cc"

using namespace std;

  // Initialized for testing purpose: running code in Windows to emulate getuserinfo() gethostname()
  string user = "user";
  char host[999] = "localhost";

void init() {
  cout << c_bold << "\n\nrShell [Version " << version << "]";
  cout << "\n\nUse \"verbose [on|off]\" to toggle verbose output.\n\n" << c_reset;
  // Unix get userinfo
  user = getlogin();
  gethostname(host, 999);
}

int newCmd() {

  // to distinguish with system shell I used "R$" instead of "$"
  cout << "\n" << c_prompt << "[" << user << "@" << host << "] R$ " << c_reset;
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);
  if (newLine == ""){
    return 0;// if empty line
  } 
  // override for testing
  bool test = false;
  if (newLine == "test1") {
    // bug exists in the commented testline; the boost/tokenizer class wont recognize \\\" in a quote.
    // but as long as there's no multiple \"s in a quote, it will function well.
    newLine = "echo \"/bin/bash\" \"/bin/bash# This Part is a\\ # inside a quote\"; echo \"Hello World\" #This is a real quote ";
    test = true;
    //newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\\" # inside a quote\"; echo \"Hello World\" #This is a real quote ";
  } else if (newLine == "test2") {
    newLine = "ls -a; echo hallo welt && mkdir test || echo world; git status; exit; ls -l";
    test = true;
  } else if (newLine == "test3") {
    newLine = "ls& ls&& ls&&& ls & ls && ls &&& ls &ls&&ls&&&ls";
    test = true;
  } else if (newLine == "test4") {
    newLine = "echo \"Hello && echo World\" && ls";
    test = true;
  }
  if (test) { cout << "\nTesting stdin override << " << newLine << endl ; }

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
  while (1) {
    newCmd();
  }
  cout << "\n\n"; // kind of... clear the screen
  return 0;
}
