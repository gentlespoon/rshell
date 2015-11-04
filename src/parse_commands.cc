#ifndef _pc_cc_
#define _pc_cc_

#include "common.hpp"

using namespace std;
using namespace boost;

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

class cmd {
protected:
  string cmdLine;
  vector<s_cmd> cmdList;
public:
  string removeComment() {
    if (V) cout << "\n======= Start Remove Comment =======\n[Raw Line] " << cmdLine << endl;
    bool isInQuote = false;

    // Finished Quotation and Comments
    // ====== bug exist ======
    // For addresses that use \ instead of / (e.g. Windows), the program may mess up with escape characters
    for (unsigned index = 0; index < cmdLine.length(); index++) {
      
      // Dealing with "" quotes
      if (cmdLine.at(index) == '\"') {
        if (V) cout << "\" found at position " << index;
        if ((index == 0) || (cmdLine.at(index-1) != '\\')) {
          if (V) cout << " This is a real quotation mark.\n";
          isInQuote = !isInQuote;
          if (V) cout << "[isInQuote] " << isInQuote << endl;
        }
        else {
          if (isInQuote) {
            if (V) cout << " This is a slashed quotation mark in a quote.\n";
          }
          else {
            cout << "\n** WARNING: There might be a slashed quotation mark outside a quote!\n";
          }
          if (V) cout << "[isInQuote] " << isInQuote << endl;
        }
      }

      // Dealing with # comments
      if (cmdLine.at(index) == '#') {
        if (V) cout << "# found at position " << index;
        if (isInQuote) {
          if (V) cout << ", however this # is in a Quote. Ignored.\n";
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
      cout << "\n** WARNING: The line ends in a quote. \" expected.\n";
    }

    if (V) cout << "[Out Line] " << cmdLine << "\n======== End Remove Comment ========\n";
    return cmdLine;
  }

  vector<s_cmd> parseCmd(string delim) {
    vector<s_cmd> p_cmdList;
    if (V) cout << "\n======= Start Parsing for " << delim << " =======\n";
    // split delim into characters
    vector<char> delim_char;
    for (unsigned i = 0; i < delim.length(); i++) {
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
    unsigned space;
    for (unsigned i = 0; i < cmdList.size(); i++) { // start parsing first line of cmdList
      string currexec = cmdList.at(i).exec;
      bool isHead = true;
      bool isInQuote = false;
      for (unsigned j = 0; j < cmdList.at(i).argv.length(); j++) { // scan thru all characters in cmdList[1].argv
        if (cmdList.at(i).argv.at(j) == '\"') {
          if (V) cout << "\" found at position " << j;
          if ((j == 0) || (cmdLine.at(j-1) != '\\')) {
            if (V) cout << " This is a real quotation mark. ";
            isInQuote = !isInQuote;
          }
          else {
            if (isInQuote) {
              if (V) cout << " This is a slashed quotation mark in a quote. ";
            }
            else {
              cout << "\n** WARNING: There might be a slashed quotation mark outside a quote!" << endl;
            }
          }
          if (V) cout << "[isInQuote]=" << isInQuote << endl;
        } else if ((!isInQuote) && (cmdList.at(i).argv.at(j) == delim_char.at(0))) { // match the first delim char
          // check the char to the front of the matched delim to make sure it is not an escaped char.
          unsigned jprev = j;
          if (jprev != 0) { jprev -= 1; }
          if (V) {
            cout << "\n\ndelim_char[0] found! Checking previous char.";
            cout << "\n" << cmdList.at(i).argv << endl;
            for (unsigned l = 0; l < (jprev); l++) {
              cout << " ";
            }
            cout << "^^" << endl;
          }
          if (cmdList.at(i).argv.at(jprev) != '\\') {
            // if first char matches
            bool match = true;
            if (V) cout << "\n\nThis is a good delim. delim_char[0] found! Start delim paring.";
            for (unsigned k = 0; k < delim_char.size(); k++) {
              if (cmdList.at(i).argv.at(j+k) == delim_char.at(k)) { // if the following delim char matches
                if (V) {
                  cout << "\n[===== i=" << i << " j=" << j << " k=" << k << " =====]";
                  cout << "\n" << cmdList.at(i).argv << endl;
                  for (unsigned l = 0; l < (j+k); l++) {
                    cout << " ";
                  }
                  cout << "^\nMatch: \"" << cmdList.at(i).argv.at(j+k) << "\" == \"" << delim_char.at(k) << "\"\n";
                  
                }
              } else { // if the following char does not match
                if (V) {
                  cout << "\n" << cmdList.at(i).argv << endl;
                  for (unsigned l = 0; l < j+k; l++) {
                    cout << " ";
                  }
                  cout << "^\nDoes not match: \"" << cmdList.at(i).argv.at(j+k) << "\" != \"" << delim_char.at(k) << "\"\n";
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
              // if (V) cout << "\ntrimmed = " << trimmed;
              cmdList.at(i).argv = cmdList.at(i).argv.substr(j+delim.length());
              trim(cmdList.at(i).argv);
              // if (V) cout << "\ncmdList.at(" << i << ").argv = " << cmdList.at(i).argv;
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
    if (V) printlist();
    return p_cmdList;
  }

/* 
  // parse with tokenizer class.
  // with bug [echo "Hello && World" will be separated to two commands.]
  // no perfect but this is the way instructor suggests
  // use as a fallback if any future problems occured in the char by char parsing function.

  vector<s_cmd> parseCmd(string delim) {
    // if parsing the first time, initialize cmdList
    if (cmdList.size() == 0) {
      s_cmd s_cmd1(";", "", cmdLine);
      cmdList.push_back(s_cmd1);
    }
    string trimmed;
    string file;
    string argv;
    unsigned space;
    if (V) cout << "\n======= Start Parsing for " << delim << " =======\n";
    vector<s_cmd> p_cmdList;
    for (unsigned i = 0; i < cmdList.size(); i++) {
      string currexec = cmdList.at(i).exec;
      bool isHead = true;
      escaped_list_separator<char> els("\\", delim, "\"");
      tokenizer<escaped_list_separator<char> > tok(cmdList.at(i).argv, els);
      for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end(); ++beg){
        if (*beg != "") {
          trimmed = *beg;
          trim(trimmed);
          if (!isHead) {
            currexec = delim;
          }
          space = trimmed.find(' ');
          file = trimmed.substr(0, space);
          argv = trimmed;
          s_cmd s_cmd2(currexec, file, argv);
          p_cmdList.push_back(s_cmd2);
          if (V) cout << "[" << s_cmd2.exec << "] [" << s_cmd2.file << "] [" << s_cmd2.argv << "]\n";
          isHead = false;
        }
      }
    }
    cmdList = p_cmdList;
    if (V) cout << "======== End Parsing for " << delim << " ========\n";
    return p_cmdList;
  }
*/


  vector<s_cmd> trimCmd() {
    if (V) cout << "\n======= start trimCmd =======\n";
    for (unsigned i = 0; i< cmdList.size(); i++) {
      if (cmdList.at(i).argv == cmdList.at(i).file) {
        cmdList.at(i).argv = "";
      }
      cmdList.at(i).argv = cmdList.at(i).argv.substr(cmdList.at(i).argv.find(' ')+1);
    }
    if (V) printlist();
    if (V) cout << "======== end trimCmd ========\n";
    return cmdList;
  }

  cmd (string newLine) {
    cmdLine = newLine;
  }
  ~cmd () {
  }
  void printline() {
    cout << this->cmdLine<< endl;
  }
  void printlist() {
    for (unsigned i = 0; i < cmdList.size(); i++) {
      cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]\n";
    }
  }
  
/*
  void generateExecCommand()
  {
    //flag to check if we run current command based on previous command
    bool previousStatus = true;
    bool runCurrentCommand = true;
    
    for (unsigned int i = 0; i < cmdList.size(); i++)
    {
      if (cmdList.at(i).exec == ";")
      {
        runCurrentCommand = true;
      }
      else if (cmdList.at(i).exec == "&&")
      {
        if (!previousStatus)
          runCurrentCommand = false;
        else
          runCurrentCommand = true;
      }
      else if (cmdList.at(i).exec == "||")
      {
        if (!previousStatus)
          runCurrentCommand = true;
        else
          runCurrentCommand = false;
      }
      if (runCurrentCommand)
      {   
        //copying vector commands into character array
        int arg1size = (cmdList.at(i).file).size();
        int arg2size = (cmdList.at(i).argv).size();
        char arg1[arg1size];
        char arg2[arg2size];
        strcpy(arg1, (cmdList.at(i).file).c_str());
        strcpy(arg2, (cmdList.at(i).argv).c_str());
        char* arg[] = {arg1, arg2, NULL};

        //set flag to see if command executed properly
        if (!exec(arg))
          previousStatus = false;
        else
          previousStatus = true;
      }
    }
  }

*/


/*
  //forking into parent and child processes in order to execute
  //passed in parse command
  bool exec(char* arg[])
  {
    int success;
    int status;
    pid_t c_pid, pid;
    c_pid = fork();
    if (c_pid == 0)
    {
      //child process running
      execvp(arg[0], arg);
      perror("exec failed");
      exit(1);
    }
    else if (c_pid > 0)
    {
      if ((pid = wait(&status)) < 0)
        perror("waiting");
      if (WIFEXITED(status))
        success = WEXITSTATUS(status);
      if (success == 0)
        return true;
      return false;
    }
    else
      perror("fork failed"); // is it.. still return true when fork failed?
    return true;
  }

*/


  // asong011 version
  void generateExecCommand() {
    //flag to check if we run current command based on previous command
    bool previousStatus = true;
    bool runCurrentCommand = true;
    
    for (unsigned int i = 0; i < cmdList.size(); i++) {
      if (cmdList.at(i).exec == ";") {
        runCurrentCommand = true;
      } else if (cmdList.at(i).exec == "&&") {
        if (!previousStatus) {
          runCurrentCommand = false;
        } else {
          runCurrentCommand = true;
        }
      } else if (cmdList.at(i).exec == "||") {
        if (!previousStatus) {
          runCurrentCommand = true;
        } else {
          runCurrentCommand = false;
        }
      }
      if (runCurrentCommand) {
        //set flag to see if command executed properly
        if (!exec(cmdList.at(i).file, cmdList.at(i).argv)) {
          previousStatus = false;
        } else {
          previousStatus = true;
        }
      }
    }
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


};

#endif
