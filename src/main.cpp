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


// ==================== tokenizing / parsing ========================
// based on boost library tokenizer.hpp


// vector<string> splitLine(string newLine) {
//   vector<string> cmds;
  
//   // typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
//   // boost::char_separator<char> sep(" ");
//   // tokenizer tokens(newLine, sep);
//   // for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
//   //   cmds.push_back(*tok_iter);
//   // }
  


//   if (DEV) {
//     cout << "\n[Tokenized commands]\n";
//     for (int i = 0; i < cmds.size(); i++) {
//       cout << cmds.at(i) << endl;
//     }
//   }
//   return cmds;
// }
// ==================== tokenizing / parsing ========================





int newCmd() {
  // to distinguish with system shell I used "R$" instead of "$"
  cout << "\n[" << user << "@" << host << "] R$ ";
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);

  // override for testing
  if (newLine == "test1") {
    // bug exists in the second testline; the boost/tokenizer class wont recognize \\\" in a quote.
    // but as long as there's no multiple \"s in a quote, it will function well.
    newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\ # inside a quote\"; echo \"Hello World\" #This is a real quote ";
    //newLine = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\\" # inside a quote\"; echo \"Hello World\" #This is a real quote ";
  } else if (newLine == "test2") {
    newLine = "ls -a; echo hello && mkdir test || echo world; git status";
  }

  cmd cmd1(newLine);
  cmd1.printline();

  cmd1.removeComment();
  cmd1.printline();
  cmd1.parse(";");
  // cmd1.printlist();
  cmd1.parse("&&");
  // cmd1.printlist();
  cmd1.parse("||");
  // cmd1.printlist();

  // ========================================================
  // up to now, the command has been parsed to this format:
  // vector<string>
  // < string, string, string, string, string, string, string... >
  // < "ls", "-l", "&&", "echo", "hello", "world", ";", "git", "status" >
  // ========================================================
  // CODES WILL BE ADDED HERE
  // I'm trying to make the commands fit in this format:
  // vector<pair<string, pair<string, string> > >
  // < [connector], < [filename], [arguments] > >
  // < ""         , < "ls"      , "-l"        > >
  // < "&&"       , < "echo"   , "hello world" > >
  // < ";"        , < "git"     , "status"    > >
  // ========================================================




  // internal command handler;
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
