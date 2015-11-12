#include <cstdlib>
#include <string>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <stddef.h>
#include <vector>
#include <sys/wait.h>

using namespace std;
using namespace boost;

char c_warning[] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
char c_bold[] = { 0x1b, '[', '1', ';', '3', '7', 'm', 0 };
char c_prompt[] = { 0x1b, '[', '1', ';', '3', '2', 'm', 0 };
char c_black[] = { 0x1b, '[', '0', ';', '3', '0', 'm', 0 };
char c_red[] = { 0x1b, '[', '0', ';', '3', '1', 'm', 0 };
char c_green[] = { 0x1b, '[', '0', ';', '3', '2', 'm', 0 };
char c_yellow[] = { 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
char c_blue[] = { 0x1b, '[', '0', ';', '3', '4', 'm', 0 };
char c_purple[] = { 0x1b, '[', '0', ';', '3', '5', 'm', 0 };
char c_cyan[] = { 0x1b, '[', '0', ';', '3', '6', 'm', 0 };
char c_white[] = { 0x1b, '[', '0', ';', '3', '7', 'm', 0 };
char c_reset[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
bool V = false;

std::string version = "20151110 Test command & Precedence Operators";
// global verbose output control added.
// can be switched by "$ verbose [on|off]" in runtime

// verbose file
string vout(string str, size_t loc) {
  ostringstream oss;
  oss << c_green;
  string blk = "-"/*"\u2588"*/;
  string vblk = "|";
  size_t l = str.length();
  size_t w = l;
  if (loc > l) w = loc;
  if (loc >= l) {
    oss << endl;
    for (size_t i = 0; i < w+3; i++) oss << blk;
    oss << endl << vblk << "loc (" << loc << ") out of range (" << l << ")" << endl;
  }
  if (!V) {}
  else {
    oss << endl;
    for (size_t i = 0; i < w+1; i++) oss << blk;
    oss << endl << str;
    for (size_t i = l; i < w+1; i++) oss << " ";
    oss << endl;
    for (size_t i = 0; i < loc; i++) oss << blk;
    oss << "^";
    for (size_t i = loc; i < w; i++) oss << blk;
    oss << endl;
  }
  oss << c_reset;
  return oss.str();
}

// parse_commands file
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

string cmdLine;
vector<s_cmd> cmdList;
void printline() {
  cout << cmdLine<< endl;
}
void printlist() {
  for (size_t i = 0; i < cmdList.size(); i++) {
    cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]" << endl;
  }
}

string removeComment() {
  if (V) cout << "\n======= Start Remove Comment =======\n[Raw Line] " << cmdLine << endl;
  bool isInQuote = false;

  // Finished Quotation and Comments
  // ====== bug exist ======
  // For addresses that use \ instead of / (e.g. Windows), the program may mess up with escape characters
  for (size_t index = 0; index < cmdLine.length(); index++) {
    
    // Dealing with "" quotes
    if (cmdLine.at(index) == '\"') {
      if (V) cout << "\n\" found at position " << index;
      cout << vout(cmdLine, index);
      if ((index == 0) || (cmdLine.at(index-1) != '\\')) {
        if (V) cout << " This is a real quotation mark.\n";
        isInQuote = !isInQuote;
        if (V) cout << "[isInQuote] " << isInQuote << endl;
      }
      else {
        if (isInQuote) {
          if (V) cout << " This is a slashed quotation mark in a quote.";
        }
        else {
          cout << c_warning << "\n** WARNING: There might be a slashed quotation mark outside a quote!" << c_reset << endl;
        }
        if (V) cout << "[isInQuote] " << isInQuote << endl;
      }
    }

    // Dealing with # comments
    if (cmdLine.at(index) == '#') {
      if (V) cout << "\n# found at position " << index;
      cout << vout(cmdLine, index);
      if (isInQuote) {
        if (V) cout << ", however this # is in a Quote. Ignored.";
      }
      else {
        if (V) cout << ", wow we found a comment!\n";
        cmdLine = cmdLine.substr(0, index);
        break;
      }
    }
  }

  // Finished Remove Comment
  if (isInQuote) { // if when the line ends we are still in a quote
    cout << c_warning << "\n** WARNING: The line ends in a quote. \" expected." << c_reset << endl;
  }

  if (V) cout << "[Out Line] " << cmdLine << "\n======== End Remove Comment ========\n";
  return cmdLine;
}

vector<s_cmd> parseCmd(string delim) {
  vector<s_cmd> p_cmdList;
  if (V) cout << "\n======= Start Parsing for " << delim << " =======";
  // split delim into characters
  vector<char> delim_char;
  for (size_t i = 0; i < delim.length(); i++) {
    delim_char.push_back(delim.at(i));
    // if (V) cout << "\n[delim = " << delim << " ] delim_char[" << i << "] = " << delim_char.at(i);
  }
  // if parsing the first time, initialize cmdList
  if (cmdList.size() == 0) {
    s_cmd s_cmd1(";", "", cmdLine);
    cmdList.push_back(s_cmd1);
  }
  string trimmed;
  string file;
  string argv;
  size_t space;
  for (size_t i = 0; i < cmdList.size(); i++) { // start parsing first line of cmdList
    string currexec = cmdList.at(i).exec;
    bool isHead = true;
    bool isInQuote = false;
    for (size_t j = 0; j < cmdList.at(i).argv.length(); j++) { // scan thru all characters in cmdList[1].argv
      if (cmdList.at(i).argv.at(j) == '\"') {
        if (V) cout << "\n\" found at position " << j;
        cout << vout(cmdList.at(i).argv, j);
        if ((j == 0) || (cmdLine.at(j-1) != '\\')) {
          if (V) cout << " This is a real quotation mark. ";
          isInQuote = !isInQuote;
        }
        else {
          if (isInQuote) {
            if (V) cout << " This is a slashed quotation mark in a quote. ";
          }
          else {
            cout << c_warning << "\n** WARNING: There might be a slashed quotation mark outside a quote!" << c_reset << endl;
          }
        }
        if (V) cout << "[isInQuote]=" << isInQuote;
      } else if ((!isInQuote) && (cmdList.at(i).argv.at(j) == delim_char.at(0))) { // match the first delim char
        // check the char to the front of the matched delim to make sure it is not an escaped char.
        size_t jprev = j;
        if (jprev != 0) { jprev -= 1; }
        if (V) {
          cout << vout(cmdList.at(i).argv, j);
          cout << "delim_char[0] found! Checking previous char.";
        }
        if (cmdList.at(i).argv.at(jprev) != '\\') {
          // if first char matches
          bool match = true;
          if (V) cout << "\nThis delim_char[0] is not escaped! Start delim matching.";
          for (size_t k = 0; k < delim_char.size(); k++) {
            if (cmdList.at(i).argv.at(j+k) == delim_char.at(k)) { // if the following delim char matches
              if (V) {
                cout << vout(cmdList.at(i).argv, j+k);
                cout <<"Match: \"" << cmdList.at(i).argv.at(j+k) << "\" == \"" << delim_char.at(k) << "\"";
                
              }
            } else { // if the following char does not match
              if (V) {
                cout << vout(cmdList.at(i).argv, j+k);
                cout << "Does not match: \"" << cmdList.at(i).argv.at(j+k) << "\" != \"" << delim_char.at(k) << "\"";
              }
              match = false;
              break; // does not match, break the loop
            }
          }
          if (match) {
          if (V) cout << "\n!! We got a " << delim << "! Constructing new command";
            // separate commands here;
            // relation with former cmd
            if (isHead) {
              isHead = false;
            } else {
              currexec = delim;
            }
            // filename
            trimmed = cmdList.at(i).argv.substr(0, j);
            trim(trimmed);
            cmdList.at(i).argv = cmdList.at(i).argv.substr(j+delim.length());
            trim(cmdList.at(i).argv);
            j = -1; // end of loop will add 1 to make it 0
            space = trimmed.find(' ');
            file = trimmed.substr(0, space);
            // if (V) cout << "\nfile = " << file;
            argv = trimmed;
            s_cmd s_cmd2(currexec, file, argv);
            p_cmdList.push_back(s_cmd2);
            if (V) cout << "\n[" << s_cmd2.exec << "] [" << s_cmd2.file << "] [" << s_cmd2.argv << "]";
            // separate commands here;
          }
        } else { // if the first char matches but it is an escaped char
          if (V) cout << " This is an escaped delimiter" << endl;
        }
      } else { // if first char does not match.
      }
    }
    //looped to last character and no more delim found
    //      push the last part of stuff into p_cmdList();
    if (isHead) {
      isHead = false;
    } else {
      currexec = delim;
    }
    if (V) cout << "\nNo more delimiters.\nDumping the rest of the line into one last command.";
    // filename
    trimmed = cmdList.at(i).argv;
    trim(trimmed);
    // if (V) cout << "trimmed = " << trimmed;
    space = trimmed.find(' ');
    file = trimmed.substr(0, space);
    // if (V) cout << "\nfile = " << file;
    argv = trimmed;
    s_cmd s_cmd3(currexec, file, argv);
    p_cmdList.push_back(s_cmd3);
    if (V) cout << "\n[" << s_cmd3.exec << "] [" << s_cmd3.file << "] [" << s_cmd3.argv << "]";
  }

  cmdList = p_cmdList;
  if (V) cout << "\n======== End Parsing for " << delim << " ========\n";
  return p_cmdList;
}

vector<s_cmd> trimCmd() {
  if (V) cout << "\n======= start trimCmd =======" << endl << "Before trim:" << endl;
  if (V) printlist();
  if (V) cout << "trimming..." << endl;
  for (size_t i = 0; i< cmdList.size(); i++) {
    if (cmdList.at(i).file == "") {
      if (V) cout << "Empty command detected at cmdList[" << i << "]. Deleting...\n";
      cmdList.erase(cmdList.begin()+i);
      i--;
    }
    if (cmdList.at(i).argv == cmdList.at(i).file) {
      cmdList.at(i).argv = "";
    } else {
      cmdList.at(i).argv = cmdList.at(i).argv.substr(cmdList.at(i).argv.find(' ')+1);
    }
  }
  if (V) cout << "trimmed:" << endl;
  if (V) printlist();
  if (V) cout << "======== end trimCmd ========\n";
  return cmdList;
}

//forking into parent and child processes in order to execute
//passed in parse command
bool exec(string file, string argv) {
  // handle rshell built-in commands;
  if (file == "verbose") {
    if ((argv == "on") || argv == "1") {
      V = true;
      cout << "Verbose output is now turned on.\nToggle: verbose [on|off]" << endl;
    } else {
      V = false;
      cout << "Verbose output is now turned off.\nToggle: verbose [on|off]" << endl;
    }
    return true;
  }
  if (file == "exit") {
    exit(0);
  }
  // handle rshell built-in commands;

  char *args[3] = {NULL, NULL, NULL};
  args[0] = const_cast<char *>(file.c_str());
  if (argv.length() != 0) {
    args[1] = const_cast<char *>(argv.c_str());
  }

  int success;
  int status;
  pid_t c_pid, pid;
  c_pid = fork();
  if (c_pid == 0) {
    //child process running
    execvp(args[0], args);
    perror("exec failed");
    exit(1);
  }
  else if (c_pid > 0) {
    if ((pid = wait(&status)) < 0) {
      perror("waiting");
    }
    if (WIFEXITED(status)) {
      success = WEXITSTATUS(status);
    }
    if (success == 0) {
      return true;
    }
    return false;
  }
  else {
    perror("fork failed"); // is it.. still return true when fork failed?
  }return true;
}

void generateExecCommand() {

  cout << c_green << "\n\n\n==================================\n" << c_reset;
  cout << c_green << "=======generateExecCommand========\n" << c_reset;
  cout << c_green << "==================================\n" << c_reset;
  
  //flag to check if we run current command based on previous command
  bool previousStatus = true;
  bool runCurrentCommand = true;
  
  for (size_t i = 0; i < cmdList.size(); i++) {
    if (V) cout << "\nWe have " << cmdList.size() << " commands, now executing command " << i+1 << endl;
    if (V) cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]\n";
    if (V) cout << "First we check the exec bit: " << cmdList.at(i).exec << endl;
    if (cmdList.at(i).exec == ";") {
      if (V) cout << "Okay, run anyway." << endl;
      runCurrentCommand = true;
    } else if (cmdList.at(i).exec == "&&") {
      if (V) cout << "Let's see if our previous command executed successfully: ";
      if (!previousStatus) {
        if (V) cout << "false. Do not execute this command." << endl;
        runCurrentCommand = false;
      } else {
        if (V) cout << "true. Execute this command." << endl;
        runCurrentCommand = true;
      }
    } else if (cmdList.at(i).exec == "||") {
      if (V) cout << "Let's see if our previous command executed successfully: ";
      if (!previousStatus) {
        if (V) cout << "false. Execute this command." << endl;
        runCurrentCommand = true;
      } else {
        if (V) cout << "true. Do not execute this command." << endl;
        runCurrentCommand = false;
      }
    }
    if (runCurrentCommand) {
      if (V) cout << "So now we decided to execute this command." << endl;
      if (V) cout << "================== EXECUTE START ==================" << endl << endl << endl;
      //set flag to see if command executed properly
      /*if (!exec(cmdList.at(i).file, cmdList.at(i).argv)) {
        previousStatus = false;
      } else {
        previousStatus = true;
      }*/ // inherit from cbui's code
      previousStatus = exec(cmdList.at(i).file, cmdList.at(i).argv);
      if (V) cout << endl << endl << endl << "=================== EXECUTE END ==================="<< endl;
      if (V) cout << "Command " << i << " executed. Success? " << previousStatus << endl;
    }
  }
}




// };







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

  // cmd cmd1(newLine);
  // cmd1.removeComment();
  // cmd1.parseCmd(";");
  // cmd1.parseCmd("&&");
  // cmd1.parseCmd("||");
  // cmd1.trimCmd();
  // cmd1.generateExecCommand();
  // cmd cmd1(newLine);
  removeComment();
  parseCmd(";");
  parseCmd("&&");
  parseCmd("||");
  trimCmd();
  generateExecCommand();
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
