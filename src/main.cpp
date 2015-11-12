#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stddef.h>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

std::string version = "20151110 Test command & Precedence Operators";

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
string user = "user";
char host[999] = "hostname";















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











string vout(string str, size_t loc) {
  ostringstream oss;
  // oss << c_green;
  string blk = "-";
  size_t l = str.length();
  size_t w = l;
  if (loc > l) w = loc;
  if (loc >= l) {
    for (size_t i = 0; i < w; i++) oss << blk;
    oss << endl << "loc (" << loc << ") out of range (" << l << ")" << endl;
  }
  if (!V) {}
  else {
    for (size_t i = 0; i < w; i++) oss << blk;
    oss << endl << str;
    for (size_t i = l; i < w; i++) oss << " ";
    oss << endl;
    for (size_t i = 0; i < loc; i++) oss << blk;
    oss << "^";
    for (size_t i = loc+1; i < w; i++) oss << blk;
    oss << endl;
  }
  // oss << c_reset;
  return oss.str();
}












void printlist(vector<s_cmd> cmdList) {
  for (size_t i = 0; i < cmdList.size(); i++) {
    cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]" << endl;
  }
}















string removeComment(string cmdLine) {
  if (V) cout << c_green;
  if (V) cout << "======= Start Remove Comment =======\n[Input Line] " << cmdLine << endl;
  bool isInQuote = false;
  for (size_t index = 0; index < cmdLine.length(); index++) {
    if (cmdLine.at(index) == '\"') {
      if (V) cout << "\" found at position " << index << endl;
      cout << vout(cmdLine, index);
      if ((index == 0) || (cmdLine.at(index-1) != '\\')) {
        if (V) cout << "This is a real quotation mark." << endl;
        isInQuote = !isInQuote;
        if (V) cout << "[isInQuote] " << isInQuote << endl;
      }
      else {
        if (isInQuote) {
          if (V) cout << "This is a slashed quotation mark in a quote." << endl;
        }
        else {
          cout << c_warning << "** WARNING: There might be a slashed quotation mark outside a quote!" << c_reset << endl;
        }
        if (V) cout << "[isInQuote] " << isInQuote << endl;
      }
    }
    // Dealing with # comments
    if (cmdLine.at(index) == '#') {
      if (V) cout << "# found at position " << index << endl;
      cout << vout(cmdLine, index);
      if (isInQuote) {
        if (V) cout << "However this # is in a Quote. Ignored." << endl;
      }
      else {
        if (V) cout << "We found a comment. substr." << endl;
        cmdLine = cmdLine.substr(0, index);
        break;
      }
    }
  }
  if (isInQuote) { // when the line ends we are still in a quote
    cout << c_warning << "** WARNING: The line ends in a quote. \" expected." << c_reset << endl;
    return "";
  }
  if (V) cout << "[Outpt Line] " << cmdLine << endl << "======== End Remove Comment ========" << endl;
  if (V) cout << c_reset;
  return cmdLine;
}
















vector<s_cmd> parseCmd(vector<s_cmd> cmdList, string delim) {
  if (V) cout << c_green;
  vector<s_cmd> p_cmdList;
  if (V) cout << "======= Start Parsing for " << delim << " =======" << endl;
  // split delim into characters
  vector<char> delim_char;
  for (size_t i = 0; i < delim.length(); i++) {
    delim_char.push_back(delim.at(i));
  }
  string trimmed;
  string file;
  string argv;
  for (size_t i = 0; i < cmdList.size(); i++) { // start parsing cmdList
    string currexec = cmdList.at(i).exec;
    bool isHead = true;
    bool isInQuote = false;
    for (size_t j = 0; j < cmdList.at(i).argv.length(); j++) { // scan thru all characters in cmdList[1].argv
      if (cmdList.at(i).argv.at(j) == '\"') {
        if (V) cout << "\" found at position " << j << endl;
        cout << vout(cmdList.at(i).argv, j);
        if ((j == 0) || (cmdList.at(i).argv.at(j-1) != '\\')) {
          if (V) cout << "This is a real quotation mark." << endl;
          isInQuote = !isInQuote;
        }
        else {
          if (isInQuote) {
            if (V) cout << "This is a slashed quotation mark in a quote." << endl;
          }
          else {
            cout << c_warning << "** WARNING: There might be a slashed quotation mark outside a quote!" << c_green << endl;
          }
        }
        if (V) cout << "[isInQuote]=" << isInQuote << endl;
      } else if ((!isInQuote) && (cmdList.at(i).argv.at(j) == delim_char.at(0))) { // match the first delim char
        // check the char to the front of the matched delim to make sure it is not an escaped char.
        size_t jprev = j;
        if (jprev != 0) { jprev -= 1; }
        if (V) {
          cout << vout(cmdList.at(i).argv, j);
          cout << "delim_char[0] " << delim_char.at(0) << " found! Checking previous char." << endl;
        }
        if (cmdList.at(i).argv.at(jprev) != '\\') {
          // if first char matches and is not escaped
          bool match = true;
          if (V) cout << "This delim_char[0] " << delim_char.at(0) << " is not escaped! Start delim matching." << endl;
          for (size_t k = 0; k < delim_char.size(); k++) {
            if ((j+k) < cmdList.at(i).argv.length()) {
              if ((cmdList.at(i).argv.at(j+k) == delim_char.at(k))) {
                if (V) {
                  cout << vout(cmdList.at(i).argv, j+k);
                  cout <<"Match: \"" << cmdList.at(i).argv.at(j+k) << "\" == \"" << delim_char.at(k) << "\"" << endl;
                }
              } else { // if the following char matched but is escaped
                if (V) {
                  cout << vout(cmdList.at(i).argv, j+k);
                  cout << "Does not match: \"" << cmdList.at(i).argv.at(j+k) << "\" != \"" << delim_char.at(k) << "\"" << endl;
                }
                match = false;
                break; // does not match, break the loop
              }
            } else {
              if (V) {
                cout << vout(cmdList.at(i).argv, j+k);
              }
              match = false;
              break; // does not match, break the loop
            }
          }
          if (match) {
            if (V) cout << "We got a " << delim << ". Constructing new command" << endl;
            // separate commands here;
            // relation with former cmd
            if (isHead) { isHead = false; } else { currexec = delim; } // if it's the first command found in a line, keep its exec. otherwise use the delim's exec
            trimmed = cmdList.at(i).argv.substr(0, j); // extract the command
            boost::trim(trimmed); // trim whitespaces.
            file = trimmed.substr(0, trimmed.find(' ')); //extract the file name from the command
            argv = trimmed; // keep filename for future parsing
            s_cmd s_cmd2(currexec, file, argv);
            p_cmdList.push_back(s_cmd2);
            if (V) cout << "[" << s_cmd2.exec << "] [" << s_cmd2.file << "] [" << s_cmd2.argv << "]" << endl;
            // get ready to parse the rest of the command
            cmdList.at(i).argv = cmdList.at(i).argv.substr(j+delim.length()); 
            boost::trim(cmdList.at(i).argv);
            j = -1; // end of loop will add 1 to make it 0
          }
        } else { // if the first char matches but it is an escaped char
          if (V) cout << " This is an escaped delimiter" << endl;
        }
      } else { // if first char does not match.
      }
    }
    // if looped to last character and no more delim found, then push the last part of stuff into p_cmdList();
    if (isHead) {
      isHead = false;
    } else {
      currexec = delim;
    }
    if (V) cout << "No more delimiters found.\nDump the rest of the line into one command." << endl;
    trimmed = cmdList.at(i).argv;
    boost::trim(trimmed);
    file = trimmed.substr(0, trimmed.find(' '));
    argv = trimmed;
    s_cmd s_cmd3(currexec, file, argv);
    p_cmdList.push_back(s_cmd3);
    if (V) cout << "[" << s_cmd3.exec << "] [" << s_cmd3.file << "] [" << s_cmd3.argv << "]" << endl;
  }
  cmdList = p_cmdList;
  if (V) cout << "======== End Parsing for " << delim << " ========" << endl;
  if (V) cout << c_reset;
  return p_cmdList;
}












vector<s_cmd> trimCmd(vector<s_cmd> cmdList) {
  if (V) cout << c_green;
  if (V) cout << "======= start trimCmd =======" << endl << "Before trim:" << endl;
  if (V) printlist(cmdList);
  if (V) cout << "trimming..." << endl;
  for (size_t i = 0; i< cmdList.size(); i++) {
    // handle emtpy commands created by "ls && && && && ls"
    if (cmdList.at(i).file == "") {
      if (V) cout << "Empty command detected at cmdList[" << i << "]. Erasing..." << endl;
      cmdList.erase(cmdList.begin()+i);
      i--;
    }
    // erasing filename from argv because we will not parse the cmdList anymore
    if (cmdList.at(i).argv == cmdList.at(i).file) { 
      cmdList.at(i).argv = "";
    } else {
      cmdList.at(i).argv = cmdList.at(i).argv.substr(cmdList.at(i).argv.find(' ')+1);
    }
  }
  if (V) cout << "After trim:  " << endl;
  if (V) printlist(cmdList);
  if (V) cout << "======== end trimCmd ========" << endl;
  if (V) cout << c_reset;
  return cmdList;
}












int is_built_in(string file, string argv) {
  // if "file argv" is a built in command then execute it and {return 0 if success, 1 if failed}, otherwise do nothing and return -1
  int executed = -1;
  // handle rshell built-in commands;
  if (file == "exit") {
    executed = 0;
    cout << c_reset << endl;
    exit(0);
  }
  if (file == "verbose") {
    if ((argv == "on") || argv == "1") {
      V = true;
      cout << "Verbose output is now turned on.\nToggle: verbose [on|off]" << endl;
    } else {
      V = false;
      cout << "Verbose output is now turned off.\nToggle: verbose [on|off]" << endl;
    }
    executed = 0;
  }
  return executed;
}














bool EXECUTE(string file, string argv) {
  if (V) cout << c_green << flush;
  int built_in = is_built_in(file, argv);
  if (built_in == -1) { // if "file argv" is not a built-in command:
    vector<string> argList;
    if (argv.length() != 0) {
      bool isInQuote = false;
      for (size_t i = 0; i < argv.length(); i++) { // tokenize argv
        if (argv.at(i) == '\"') {
          if (V) cout << "\" found at position " << i << endl;
          cout << vout(argv, i);
          if ((i == 0) || (argv.at(i-1) != '\\')) {
            if (V) cout << "This is a real quotation mark." << endl;
            argv.erase(argv.begin() + i);
            i--;
            isInQuote = !isInQuote;
            if (V) cout << "[isInQuote]=" << isInQuote << endl;
          }
          else {
            if (isInQuote) {
              if (V) cout << "This is a slashed quotation mark in a quote." << endl;
            }
          }
        } else if (argv.at(i) == ' ') {
          cout << vout(argv, i);
          if (!isInQuote) {
            if (V) cout << "This is a real space. Tokenize." << endl;
            argList.push_back(argv.substr(0, i));
            argv = argv.substr(i+1);
            i=-1;
          } else {
            if (V) cout << "This is a space in quote. Ignore." << endl;
          }
        }
      }
      argList.push_back(argv);
      if (V) {
        cout << "List of arguments:" << endl;
        for (size_t j = 0; j < argList.size(); j++) {
          cout << argList.at(j) << endl;
        }
      }
    } else {
      if (V) cout << "No arguments provided." << endl;
    }

    char **args = new char*[argList.size()+2];
    args[0] = const_cast<char*>(file.c_str());
    size_t i;
    for (i = 0; i < argList.size(); i++) {
      args[i+1] = const_cast<char*>(argList.at(i).c_str());
    }
    args[i+1] = NULL;
    if (V) cout << c_reset << flush;
    int success;
    int status;
    pid_t c_pid, pid;
    c_pid = fork();
    if (c_pid == 0) {
      //child process running
      execvp(args[0], args);
      perror("exec failed");
      delete[] args;
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
        delete[] args;
        return true;
      }
      delete[] args;
      return false;
    }
    else {
      perror("fork failed");
      delete[] args;
      exit(1);
    }

    delete[] args;
  } else if (built_in == 1) {
    return true;
  } else {
    return false;
  }
  return true;
}















void execCommand(vector<s_cmd> cmdList) {
  if (V) cout << c_green << "=======generateExecCommand========" << endl;
  // flags to check if we run current command based on previous command
  bool previousStatus = true;
  bool runCurrentCommand = true;
  for (size_t i = 0; i < cmdList.size(); i++) {
    if (V) cout << "We have " << cmdList.size() << " commands, now executing command " << i+1 << endl;
    if (V) cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]" << endl;
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
      if (V) cout << "================== EXECUTE START ==================" << c_reset << endl;
      previousStatus = EXECUTE(cmdList.at(i).file, cmdList.at(i).argv);
      if (V) cout << c_green <<"=================== EXECUTE END ===================" << endl;
      if (V) cout << "Command " << i << " executed. isSuccess? " << previousStatus << endl;
    }
  }
  if (V) cout << c_reset;
}















int newCmd() {
  // to distinguish with system shell I used "R$" instead of "$"
  cout << c_prompt << "" << user << "@" << host << " R$ " << c_reset;
  // GET USER INPUT

  string newLine;
  getline(cin, newLine);
  // override for testing
  bool test = false;
  if (newLine == "test1") {
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
  if (test) { cout << "Testing stdin override << " << newLine << endl; }

  newLine = removeComment(newLine);
  if (newLine == ""){
    return 0;// if empty line or quotation error
  } 
  vector<s_cmd> cmdList;
  s_cmd s_cmd1(";", "", newLine);
  cmdList.push_back(s_cmd1);
  cmdList = parseCmd(cmdList, ";");
  cmdList = parseCmd(cmdList, "&&");
  cmdList = parseCmd(cmdList, "||");
  cmdList = trimCmd(cmdList);
  execCommand(cmdList);
  return 0;
}













int main(int argc, char *argv[]) {
  cout << c_bold << "\n\nrShell [Version " << version << "]" << endl;
  cout << "Use \"verbose [on|off]\" to toggle verbose output." << c_reset << endl << endl;
  user = getlogin();
  gethostname(host, 999);
  while (1) {
    newCmd();
  }
  cout << endl << endl; // kind of... clear the screen
  return 0;
}
