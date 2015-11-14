#include "common.hpp"
#include "asutil.hpp"
#include "built_in_cmds.cpp"

using namespace std;



/*
             .                                      .            
           .o8                                    .o8            
 .oooo.o .o888oo oooo d8b oooo  oooo   .ooooo.  .o888oo  .oooo.o 
d88(  "8   888   `888""8P `888  `888  d88' `"Y8   888   d88(  "8 
`"Y88b.    888    888      888   888  888         888   `"Y88b.  
o.  )88b   888 .  888      888   888  888   .o8   888 . o.  )88b 
8""888P'   "888" d888b     `V88V"V8P' `Y8bod8P'   "888" 8""888P' 
*/
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





/*
    .                       oooo  
  .o8                       `888  
.o888oo  .ooooo.   .ooooo.   888  
  888   d88' `88b d88' `88b  888  
  888   888   888 888   888  888  
  888 . 888   888 888   888  888  
  "888" `Y8bod8P' `Y8bod8P' o888o 
*/
string printlist(vector<s_cmd> cmdList, size_t pos = SIZE_MAX) {
  ostringstream oss;
  if (pos == SIZE_MAX) {
    for (size_t i = 0; i < cmdList.size(); i++) {
      oss << i << "\t[" << cmdList.at(i).exec << "]\t[" << cmdList.at(i).file << "]\t[" << cmdList.at(i).argv << "]" << endl;
    }
  } else {
    if (pos >= cmdList.size()) {
      oss << color("red", "bold");
      oss << endl << "pos (" << pos << ") out of range (" << cmdList.size() << ")" << endl;
      // oss << color();
    }
    for (size_t i = 0; i < cmdList.size(); i++) {
      oss << i;
      if (i==pos) {
        oss << " -->\t";
      }
      else {
        oss << "\t";
      }
      oss << " [" << cmdList.at(i).exec << "]\t[" << cmdList.at(i).file << "]\t[" << cmdList.at(i).argv << "]" << endl;
    }
  }
  return oss.str();

}









/*
                                        o8o                         
                                        `"'                         
oo.ooooo.   .oooo.   oooo d8b  .oooo.o oooo  ooo. .oo.    .oooooooo 
 888' `88b `P  )88b  `888""8P d88(  "8 `888  `888P"Y88b  888' `88b  
 888   888  .oP"888   888     `"Y88b.   888   888   888  888   888  
 888   888 d8(  888   888     o.  )88b  888   888   888  `88bod8P'  
 888bod8P' `Y888""8o d888b    8""888P' o888o o888o o888o `8oooooo.  
 888                                                     d"     YD  
o888o                                                    "Y88888P'  
*/


string removeComment(string cmdLine) {
  // if (V) cout << color("green");
  if (V) cout << "======= Start Remove Comment =======\n[Input Line] " << cmdLine << endl;
  bool isInQuote = false;
  for (size_t index = 0; index < cmdLine.length(); index++) {
  if (cmdLine.at(index) == '\"') {
    if (V) cout << "\" found at position " << index << endl;
    if (V) cout << str_pos(cmdLine, index);
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
      if (V) cout << color("yellow") << "** Notice: There might be a slashed quotation mark outside a quote." << color("green") << endl; // downgrade from WARNING to NOTICE
    }
    if (V) cout << "[isInQuote] " << isInQuote << endl;
    }
  } else if ((!isInQuote) && (cmdLine.at(index) == '#')) { // Dealing with # comments when not in a quote
    if (V) cout << "# found at position " << index << endl;
    if (V) cout << str_pos(cmdLine, index);
    // if (isInQuote) {
    // if (V) cout << "However this # is in a Quote. Ignored." << endl;
    // }
    // else {
    if (V) cout << "We found a comment. substr." << endl;
    cmdLine = cmdLine.substr(0, index);
    break;
    // }
  }
  }
  if (isInQuote) { // when the line ends we are still in a quote
  cout << color("red", "b") << "** WARNING: The line ends in a quote. \" expected." << color("green") << endl;
  return "";
  }
  if (V) cout << "[Outpt Line] " << cmdLine << endl << "======== End Remove Comment ========" << endl;
  // if (V) cout << color() << flush;
  return cmdLine;
}










vector<s_cmd> parseCmd(vector<s_cmd> cmdList, string delim) {
  // if (V) {cout << color("green");}
  vector<s_cmd> p_cmdList;
  if (V) {cout << "======= Start Parsing for " << delim << " =======" << endl;}
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
        if (V) {cout << "\" found at position " << j << endl;}
        if (V) cout << str_pos(cmdList.at(i).argv, j);
        if ((j == 0) || (cmdList.at(i).argv.at(j-1) != '\\')) {
          if (V) {cout << "This is a real quotation mark." << endl;}
          isInQuote = !isInQuote;
        }
        else {
          if (isInQuote) {
            if (V) {cout << "This is a slashed quotation mark in a quote." << endl;}
          }
        }
        if (V) {cout << "[isInQuote]=" << isInQuote << endl;}
      } else if ((!isInQuote) && (cmdList.at(i).argv.at(j) == delim_char.at(0))) { // match the first delim char
        // check the char to the front of the matched delim to make sure it is not an escaped char.
        size_t jprev = j;
        if (jprev != 0) { jprev -= 1; }
        if (V) {
          cout << str_pos(cmdList.at(i).argv, j);
          cout << "delim_char[0] " << delim_char.at(0) << " found! Checking previous char." << endl;
        }
        if (cmdList.at(i).argv.at(jprev) != '\\') {
          // if first char matches and is not escaped
          bool match = true;
          if (V) {cout << "This delim_char[0] " << delim_char.at(0) << " is not escaped! Start delim matching." << endl;}
          for (size_t k = 0; k < delim_char.size(); k++) {
            if ((j+k) < cmdList.at(i).argv.length()) {
              if ((cmdList.at(i).argv.at(j+k) == delim_char.at(k))) {
                if (V) {
                  cout << str_pos(cmdList.at(i).argv, j+k);
                  cout <<"Match: \"" << cmdList.at(i).argv.at(j+k) << "\" == \"" << delim_char.at(k) << "\"" << endl;
                }
              } else { // if the following char matched but is escaped
                if (V) {
                  cout << str_pos(cmdList.at(i).argv, j+k);
                  cout << "Does not match: \"" << cmdList.at(i).argv.at(j+k) << "\" != \"" << delim_char.at(k) << "\"" << endl;
                }
                match = false;
                break; // does not match, break the delim matching loop
              }
            } else {
              if (V) {
                cout << str_pos(cmdList.at(i).argv, j+k);
              }
              match = false;
              break; // does not match, break the loop
            }
          }
          if (match) {
            if (V) {cout << "We got a " << delim << ". Constructing new command" << endl;}
            // separate commands here;
            // relation with former cmd
            if (isHead) { isHead = false; } else { currexec = delim; } // if it's the first command found in a line, keep its exec. otherwise use the delim's exec
            trimmed = cmdList.at(i).argv.substr(0, j); // extract the command
            boost::trim(trimmed); // trim whitespaces.
            file = trimmed.substr(0, trimmed.find(' ')); //extract the file name from the command
            argv = trimmed; // keep filename for future parsing
            s_cmd parsing_cmd(currexec, file, argv);
            p_cmdList.push_back(parsing_cmd);
            if (V) {cout << "[" << parsing_cmd.exec << "] [" << parsing_cmd.file << "] [" << parsing_cmd.argv << "]" << endl;}
            // get ready to parse the rest of the command
            cmdList.at(i).argv = cmdList.at(i).argv.substr(j+delim.length()); 
            boost::trim(cmdList.at(i).argv);
            j = -1; // end of loop will add 1 to make it 0
          }
        } else { // if the first char matches but it is an escaped char
          if (V) {cout << " This is an escaped delimiter" << endl;}
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
    if (V) {cout << "No more delimiters found.\nDump the rest of the line into one command." << endl;}
    trimmed = cmdList.at(i).argv;
    boost::trim(trimmed);
    file = trimmed.substr(0, trimmed.find(' '));
    argv = trimmed;
    s_cmd parsing_cmd(currexec, file, argv);
    p_cmdList.push_back(parsing_cmd);
    if (V) {cout << "[" << parsing_cmd.exec << "] [" << parsing_cmd.file << "] [" << parsing_cmd.argv << "]" << endl;}
  }
  cmdList = p_cmdList;
  if (V) {cout << "======== End Parsing for " << delim << " ========" << endl;}
  if (V) {cout << printlist(cmdList);}
  // if (V) {cout << color() << flush;}
  return p_cmdList;
}




vector<s_cmd> f_parenthesis(vector<s_cmd> cmdList) {
  if (V) cout << "====== Start parenthesis fix ======\nBefore:" << endl;
  if (V) cout << printlist(cmdList);
  for(size_t i = 0; i< cmdList.size(); i++) {
    if (cmdList.at(i).exec == "(") {
      s_cmd f_p_cmd(";", cmdList.at(i).file, cmdList.at(i).argv);
      cmdList.insert(cmdList.begin()+i+1, f_p_cmd);
      cmdList.at(i).file = "";
      cmdList.at(i).argv = "";
    }
  }
  if (V) cout << "After:" << endl;
  if (V) cout << printlist(cmdList);
  if (V) cout << "======= End parenthesis fix =======" << endl;
  return cmdList;
}






vector<s_cmd> trimCmd(vector<s_cmd> cmdList) {
  // if (V) cout << color("green");
  if (V) cout << "======= Start trimCmd =======" << endl << "Before trim:" << endl;
  if (V) cout << printlist(cmdList);
  if (V) cout << "-------trimming-------" << endl;
  for (int i = cmdList.size()-1; i >= 0 ; i--) {
    if (V) cout << "Checking cmdList[" << i << "]..." << endl;
    // if (V) cout << printlist(cmdList);

    // remove empty commands
    /*if (cmdList.at(i).file == "") {
      if ((cmdList.at(i).exec != "(") && (cmdList.at(i).exec != ")")) {
        if (V) cout << "Empty command detected at cmdList[" << i << "]. Erasing..." << endl;
        // if (V) cout << "====== size " << cmdList.size() << endl;
        cmdList.erase(cmdList.begin()+i);
        // if (V) cout << "Erased" << endl;
      }
    } else */if (cmdList.at(i).argv == cmdList.at(i).file) { // erasing filename from argv because we will not parse the cmdList anymore
      cmdList.at(i).argv = ""; // if No arguments 
    } else {
      cmdList.at(i).argv = cmdList.at(i).argv.substr(cmdList.at(i).argv.find(' ')+1); // trim filename out of arguments
    }
  }
  if (V) cout << "After trim:  " << endl;
  if (V) cout << printlist(cmdList);
  if (V) cout << "======== end trimCmd ========" << endl;
  // if (V) cout << color() << flush;
  return cmdList;
}









/*
                                                          .             
                                                        .o8             
 .ooooo.  oooo    ooo  .ooooo.   .ooooo.  oooo  oooo  .o888oo  .ooooo.  
d88' `88b  `88b..8P'  d88' `88b d88' `"Y8 `888  `888    888   d88' `88b 
888ooo888    Y888'    888ooo888 888        888   888    888   888ooo888 
888    .o  .o8"'88b   888    .o 888   .o8  888   888    888 . 888    .o 
`Y8bod8P' o88'   888o `Y8bod8P' `Y8bod8P'  `V88V"V8P'   "888" `Y8bod8P' 
*/



bool EXECUTE(string file, string argv) {
  vector<string> argList;
  if (argv.length() != 0) {
    argList = tokenize(argv);
    if (V) {
      // cout << color("green") << flush;
      cout << "List of arguments:" << endl;
      for (size_t j = 0; j < argList.size(); j++) {
        cout << argList.at(j) << endl;
      }
    }
  } else {
    if (V) cout << "No arguments provided." << endl;
  }
  if (V) cout << "Check if it is internal command." << endl;
  int built_in = is_built_in(file, argList);
  if (built_in == 1) { // if "file argv" is not a built-in command:
    if (V) cout << "Not an internal command." << endl;
    // convert (argv) vector<string> to char** for execvp
    char **args = new char*[argList.size()+2];
    args[0] = const_cast<char*>(file.c_str());
    size_t i;
    for (i = 0; i < argList.size(); i++) {
      args[i+1] = const_cast<char*>(argList.at(i).c_str());
    }
    args[i+1] = NULL;
    if (V) cout << "================== EXECUTE START ==================" << endl;
    cout << color() << flush; 
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
  } else if (built_in == 0) { // is built-in cmd and success
    return true;
  } else { // is built-in cmd but failed
    return false;
  }
  return true;
}













bool execCommand(vector<s_cmd> cmdList) {
  // if (V) cout << color("green") << flush;
  if (V) cout << "=======generateExecCommand========" << endl;
  // flags to check if we run current command based on previous command
  bool previousStatus = true;
  bool runCurrentCommand = true;
  for (size_t i = 0; i < cmdList.size(); i++) {
    if (V) cout << "We have " << cmdList.size() << " commands, now executing command " << i+1 << endl;
    // if (V) cout << "[" << cmdList.at(i).exec << "] [" << cmdList.at(i).file << "] [" << cmdList.at(i).argv << "]" << endl;
    if (V) cout << printlist(cmdList, i);
    if (V) cout << "First we check the exec bit: " << cmdList.at(i).exec << endl;
    // construct recursive function based on ()s
    int parenthesis_count = 0;
    if (V) cout << "Now checking the exec bit of command " << i << endl;
    if (V) cout << printlist(cmdList, i);
    if (cmdList.at(i).exec == "(") {
      if (V) {cout << "Oooops we found a ( operator, push all commands before corresponding ) into a new cmdList and recurse execCommand." << endl;}
      i++;
      vector<s_cmd> child_cmdList;
      for (size_t index = 0; i < cmdList.size(); index++, i++) { 
        if (V) cout << "Checking the exec bit of commands in parenthesis: " << cmdList.at(i).exec << endl;
        if (cmdList.at(i).exec != ")") {
          if (V) cout << "Get one, pushing into child_cmdList." << endl;
          if (cmdList.at(i).exec == "(") {
            if (V) cout << "Woow woow woow, another parenthesis? No problem, marked it!" << endl;
            parenthesis_count++;
          }
          s_cmd child_cmd(cmdList.at(i).exec, cmdList.at(i).file, cmdList.at(i).argv);
          child_cmdList.push_back(child_cmd);
          if (V) cout << "Pushed in." << endl;
          if (V) cout << printlist(child_cmdList);
        } else {
          if (V) cout << "We found a ) operator at " << i << "." << endl;
          if (V) cout << printlist(cmdList, i);
          parenthesis_count--;
          s_cmd child_cmd(cmdList.at(i).exec, cmdList.at(i).file, cmdList.at(i).argv);
          child_cmdList.push_back(child_cmd);
          if (V) cout << "Pushed in." << endl;
          if (V) cout << printlist(child_cmdList);
          if (V) cout << "Check parenthesis_count to see if this is the corresponding parenthesis: " << parenthesis_count << endl;
          if (parenthesis_count >= 0) {
            if (V) cout << "No it is not the corresponding parenthesis. Keep pushing." << endl;
          } else {
            if (V) cout << "Yes it is the corresponding parenthesis. Prepare for executing." << endl;
            if (V) cout << "Check runCurrentCommand to see if we need to run this part: " << runCurrentCommand << endl;
            if (runCurrentCommand) {
              if (V) cout << "Ok, enter recursive function." << endl;
              previousStatus = execCommand(child_cmdList);
            } else {
              if (V) cout << "No, we don't need to run this part. Ignore and Next." << endl;
            }
            break;
          }
        }
        // if (V) cout << "cmdList.size() = " << cmdList.size() << "    index = " << index << "    i = " << i << endl;
      }


    } else if (cmdList.at(i).exec == ";") {
      if (V) cout << "Okay, run anyway." << endl;
      runCurrentCommand = true;
    } else if (cmdList.at(i).exec == "&&") {
      if (V) cout << "Let's see if our previous command executed successfully: ";
      if (!previousStatus) {
        if (V) cout << "false. Do not execute next command." << endl;
        runCurrentCommand = false;
      } else {
        if (V) cout << "true. Execute next command." << endl;
        runCurrentCommand = true;
      }
    } else if (cmdList.at(i).exec == "||") {
      if (V) cout << "Let's see if our previous command executed successfully: ";
      if (!previousStatus) {
        if (V) cout << "false. Execute next command." << endl;
        runCurrentCommand = true;
      } else {
        if (V) cout << "true. Do not execute next command." << endl;
        runCurrentCommand = false;
      }
    }
    try {
      if (cmdList.at(i).file == "") {
        if (V) cout << "Empty line. Ignored. Next." << endl;
      } else if (runCurrentCommand) {
        if (V) cout << "So now we decided to execute next command." << endl;
        previousStatus = EXECUTE(cmdList.at(i).file, cmdList.at(i).argv);
        if (V) cout << color("green") << flush;
        if (V) cout <<"=================== EXECUTE END ===================" << endl;
        if (V) cout << "Command " << i << " executed. isSuccess? " << previousStatus << endl;
      }
    } catch (std::exception const& e) {
      cout << color("red", "bold");
      cout << "Syntax error" << endl;
      cout << color("green");
    }
  }
  if (V) cout << "Exiting function execCommand()" << endl;
  return previousStatus;
}








/*
 .o88o.                                                
 888 `"                                                
o888oo  oooo d8b  .oooo.   ooo. .oo.  .oo.    .ooooo.  
 888    `888""8P `P  )88b  `888P"Y88bP"Y88b  d88' `88b 
 888     888      .oP"888   888   888   888  888ooo888 
 888     888     d8(  888   888   888   888  888    .o 
o888o   d888b    `Y888""8o o888o o888o o888o `Y8bod8P' 
*/


int newCmd() {
  // to distinguish with system shell I use "[R]$" instead of "$"
  cout << color("green", "bold") << "" << user << "@" << host << flush;
  cout << color() << ":" << flush;
  cout << color("blue", "bold") << dir << flush;
  cout << color() << "[R]$ " << flush;
  
  // GET USER INPUT
  string newLine;
  getline(cin, newLine);
  if (newLine == ""){
    return 0;// if empty line
  } 
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
    newLine = "ls & ls && ls";
    test = true;
  } else if (newLine == "test4") {
    newLine = "echo \"Hello && echo World\" && ls";
    test = true;
  } else if (newLine == "test5") {
    newLine = "(echo A && echo B) || (echo C && echo D)";
    test = true;
  }
  if (test) { 
    cout << color("yellow", "b") << "Test stdin override << ";
    cout << color("yellow", "r") << newLine << endl;
    cout << color() << flush;
  }

  cout << color("green") << flush;
  newLine = removeComment(newLine);
  if (newLine == ""){
    return 0;// if quotation error
  } 
  vector<s_cmd> cmdList;
  s_cmd s_cmd1(";", "", newLine);
  cmdList.push_back(s_cmd1);
  cmdList = parseCmd(cmdList, "(");
  cmdList = parseCmd(cmdList, ")");
  cmdList = parseCmd(cmdList, ";");
  cmdList = parseCmd(cmdList, "&&");
  cmdList = parseCmd(cmdList, "||");
  cmdList = f_parenthesis(cmdList);
  cmdList = trimCmd(cmdList);
  execCommand(cmdList);
  return 0;
}








int main(int argc, char *argv[]) {
  cout << color("yellow", "b");
  cout << "\n\nrShell [Version " << version << "]" << endl;
  cout << color();
  cout << "Use \"verbose [on|off]\" to toggle verbose output."  << endl << endl;
  user = getlogin();
  gethostname(host, 999);
  getcwd(chardir,BUFSIZ);
  dir = chardir;
  home = charh;
  dir = str_swap(dir, home, "~");
  // cout << home << endl;
  // cout << dir << endl;
  while (1) {
    newCmd();
  }
  cout << endl << endl; // kind of... clear the screen
  return 0;
}