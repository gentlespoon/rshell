////////asdfsdfghjklsgdkajsdhfl;kjdfal;sdkfja;sldkfja;sldkjgasj



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
    // if did not specify a position
    // actually this part can be deleted because in the other branch if pos == SIZE_MAX it will never print -->
    for (size_t i = 0; i < cmdList.size(); i++) {
      oss << i << "\t[" << cmdList.at(i).exec << "]\t[" << cmdList.at(i).file << "]\t[" << cmdList.at(i).argv << "]" << endl;
    }
  } else {
    // if specified a position
    if (pos >= cmdList.size()) {
      // if position out of range
      // warn but not break
      oss << color("red");
      oss << endl << "pos (" << pos << ") out of range (" << cmdList.size() << ")" << endl;
    }
    for (size_t i = 0; i < cmdList.size(); i++) {
      // loop and print list element
      oss << i;
      if (i==pos) {
        // if this one is at the specified position
        oss << " -->\t";
        // special output with -->
      }
      else {
        // if this one is not at the specified position
        oss << "\t";
        // regular output
      }
      oss << " [" << cmdList.at(i).exec << "]\t[" << cmdList.at(i).file << "]\t[" << cmdList.at(i).argv << "]" << endl;
      // print the list
    }
  }
  return oss.str();
  // return the oss outstream
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
  if (V) {
    cout << "======= Start Remove Comment =======\n[Input Line] " << cmdLine << endl;
  }
  bool isInQuote = false;
  // set isInQuote bit to track if we are in a quote

  for (size_t index = 0; index < cmdLine.length(); index++) {
    // loop through the entire cmdLine to check every single character
    if (cmdLine.at(index) == '\"') {
      // if current char == "
      if (V) {
        cout << "\" found at position " << index << endl;
        cout << str_pos(cmdLine, index);
      }
      // print out the string and the position of the char
      if ((index == 0) || (cmdLine.at(index-1) != '\\')) {
        // if this is not the first character in a string and the char before this char is not "\ "
        if (V) {
          cout << "This is a real quotation mark." << endl;
        }
        isInQuote = !isInQuote;
        // flip the isInQuote bit to indicate quotation status change
        if (V) {
          cout << "[isInQuote] " << isInQuote << endl;
        }
      } else {
        // if the char before this char is "\ "
        if (isInQuote) {
          // if we are in a quote
          if (V) {
            cout << "This is a slashed quotation mark in a quote." << endl;
          }
        } else {
          // if we are not in a quote
          cout << color("yellow") << "** Notice: There might be a slashed quotation mark outside a quote." << color("green") << endl;
          // downgrade from WARNING to NOTICE
        }
        if (V) {
          cout << "[isInQuote] " << isInQuote << endl;
        }
      }
    }

    else if ((!isInQuote) && (cmdLine.at(index) == '#')) {
      // when we are in a quote, skip this part
      // when we are not in a quote, deal with #
      if (V) {
        cout << "# found at position " << index << endl;
        cout << str_pos(cmdLine, index);
        // print out the string and the position of the char
        cout << "We found a comment. substr." << endl;
      }
      cmdLine = cmdLine.substr(0, index);
      // cut off the comment
      break;
      // as the following are comments and are cut off, we do not need to proceed anymore
      // break out of the loop
    }
  }
  if (isInQuote) {
    // if the line ends and we are still in a quote
    cout << color("red") << flush;
    cout << "Syntax error: The line ends in a quote. \" expected."<< endl;
    cout << str_pos(cmdLine, cmdLine.length());
    // print out the string and the position of the char
    cout << color("green") << flush;
    return "";
    // return an empty line to indicate there's an error
  }
  if (V) {
    cout << "[Outpt Line] " << cmdLine << endl << "======== End Remove Comment ========" << endl;
  }
  return cmdLine;
  // return cmdLine with comments removed
}










vector<s_cmd> parseCmd(vector<s_cmd> cmdList, string delim) {
  vector<s_cmd> p_cmdList;
  // create a current working cmdList buffer
  if (V) {
    cout << "======= Start Parsing for " << delim << " =======" << endl;
  }
  
  // split delim into characters
  vector<char> delim_char;
  for (size_t i = 0; i < delim.length(); i++) {
    delim_char.push_back(delim.at(i));
  }

  string trimmed;
  string file;
  string argv;
  for (size_t i = 0; i < cmdList.size(); i++) {
    // loop through every line in cmdList.
    string currexec = cmdList.at(i).exec;
    // save the current exec bit
    bool isHead = true;
    // is it the first command we found in a line? true
    bool isInQuote = false;
    // are we in a quote? false
    for (size_t j = 0; j < cmdList.at(i).argv.length(); j++) {
      // scan thru all characters in cmdList[i].argv
      if (cmdList.at(i).argv.at(j) == '\"') {
        // if char == "
        if (V) {
          cout << "\" found at position " << j << endl;
          cout << str_pos(cmdList.at(i).argv, j);
          // print out the string and the position of the char
        }
        if ((j == 0) || (cmdList.at(i).argv.at(j-1) != '\\')) {
          // if this is not the first char and the char before this char is not "\ "
          if (V) {
            cout << "This is a real quotation mark." << endl;
          }
          isInQuote = !isInQuote;
          // flip the isInQuote bit
        } else {
          // if we are already in a quote
          if (isInQuote) {
            if (V) {
              cout << "This is a slashed quotation mark in a quote." << endl;
            }
          }
        }
        if (V) {
          cout << "[isInQuote]=" << isInQuote << endl;
        }
      }

      else if ((!isInQuote) && (cmdList.at(i).argv.at(j) == delim_char.at(0))) {
        // we are not in a quote and the first delim char matches

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
          // set the match bit = true. if any char before the end of delim doesnt match: match = false
          if (V) {
            cout << "This delim_char[0] " << delim_char.at(0) << " is not escaped! Start delim matching." << endl;
          }
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
            if (V) {
              cout << "We got a " << delim << ". Constructing new command" << endl;
            }
            
            // separate commands here;

            // relation with former cmd
            if (isHead) {
              // if this is the first command we found in a line
              isHead = false;
              // set isHead to false so all later command will not inherit the original exec
            } else {
              // if this is not the first command, use the delim's exec
              currexec = delim;
            }
            
            trimmed = cmdList.at(i).argv.substr(0, j);
            // extract the command
            boost::trim(trimmed);
            // trim whitespaces.
            file = trimmed.substr(0, trimmed.find(' '));
            //extract the file name from the command
            argv = trimmed;
            // keep filename in the argv for future parsing
            s_cmd parsing_cmd(currexec, file, argv);
            // construct a new s_cmd for pushing back into the p_cmdList
            p_cmdList.push_back(parsing_cmd);
            // push back into p_cmdList
            if (V) {
              cout << "[" << parsing_cmd.exec << "] [" << parsing_cmd.file << "] [" << parsing_cmd.argv << "]" << endl;
            }
            
            // get ready to parse the rest of the command
            cmdList.at(i).argv = cmdList.at(i).argv.substr(j+delim.length()); 
            // cut off the processed command
            boost::trim(cmdList.at(i).argv);
            // trim whitespaces
            j = -1;
            // end of loop will add 1 to make it 0 so we start to process the new cmdList
          }
        } else {
          // if the first char matches but it is an escaped char
          if (V) {
            cout << " This is an escaped delimiter" << endl;
          }
        }
      } else {
        // if first char does not match.
      }
    }
    
    if (isHead) {
      // if looped to last character and no more delim found, then push the last part of stuff into p_cmdList();
      isHead = false;
    } else {
      // if is not the first command. do not inherit the currexec
      currexec = delim;
    }
    if (V) {
      cout << "No more delimiters found.\nDump the rest of the line into one command." << endl;
    }
    
    trimmed = cmdList.at(i).argv;
    // extract command
    boost::trim(trimmed);
    // trim whitespace
    file = trimmed.substr(0, trimmed.find(' '));
    // extract filename
    argv = trimmed;
    // dump argv
    s_cmd parsing_cmd(currexec, file, argv);
    // construct new s_cmd
    p_cmdList.push_back(parsing_cmd);
    // push back
    if (V) {
      cout << "[" << parsing_cmd.exec << "] [" << parsing_cmd.file << "] [" << parsing_cmd.argv << "]" << endl;
    }
  }
  
  cmdList = p_cmdList;
  // ... what for? it doesnt make sense to assign it back to the cmdList in this scope? consider delete

  if (V) {
    cout << "======== End Parsing for " << delim << " ========" << endl;
    cout << printlist(cmdList);
  }
  
  return p_cmdList;
}




vector<s_cmd> f_parenthesis(vector<s_cmd> cmdList) {
  if (V) {
    cout << "====== Start parenthesis fix ======\nBefore:" << endl;
    cout << printlist(cmdList);
  }

  ///////////// ACTUALLY WE CAN CHECK FOR && ( ) && SYNTAX HERE, INSTEAD OF AT RUNTIME // CONSIDER /////////////////
  
  for(size_t i = 0; i < cmdList.size(); i++) {
    // loop through the entire cmdList to find (
    if (cmdList.at(i).exec == "(") {
      // if a command's exec bit is ( we separate it to its own command and set the original command's exec bit to ;
      s_cmd f_p_cmd(";", cmdList.at(i).file, cmdList.at(i).argv);
      // make a new command containing the original command's file and argv
      cmdList.insert(cmdList.begin()+i+1, f_p_cmd);
      // insert after ( command
      cmdList.at(i).file = "";
      cmdList.at(i).argv = "";
      // clear the ( command
    }
  }

  if (V) {
    cout << "After:" << endl;
    cout << printlist(cmdList);
    cout << "======= End parenthesis fix =======" << endl;
  }
  
  return cmdList;
}






vector<s_cmd> trimCmd(vector<s_cmd> cmdList) {
  if (V) {
    cout << "======= Start trimCmd =======" << endl << "Before trim:" << endl;
    cout << printlist(cmdList);
    cout << "-------trimming-------" << endl;
  }
  for (int i = cmdList.size()-1; i >= 0 ; i--) {
    if (V) {
      cout << "Checking cmdList[" << i << "]..." << endl;
    }

    // This part was commented because we need empty lines to deal with () operators...
    // Need to revisit this part to fixed && && bug.

    // if (V) cout << printlist(cmdList);
    // remove empty commands
    /*if (cmdList.at(i).file == "") {
      if ((cmdList.at(i).exec != "(") && (cmdList.at(i).exec != ")")) {
        if (V) cout << "Empty command detected at cmdList[" << i << "]. Erasing..." << endl;
        // if (V) cout << "====== size " << cmdList.size() << endl;
        cmdList.erase(cmdList.begin()+i);
        // if (V) cout << "Erased" << endl;
      }
    } else */

    // erase filename from argv because we will not parse the cmdList anymore
    if (cmdList.at(i).argv == cmdList.at(i).file) {
      // if No arguments 
      cmdList.at(i).argv = "";
    } else {
      // if there's argument
      cmdList.at(i).argv = cmdList.at(i).argv.substr(cmdList.at(i).argv.find(' ')+1);
      // trim filename out of arguments
    }
  }
  
  if (V) {
    cout << "After trim:  " << endl;
    cout << printlist(cmdList);
    cout << "======== end trimCmd ========" << endl;
  }

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


bool EXECUTE(string file, string argv = "", string rdfile = "") {
  vector<string> argList;
  if (argv.length() != 0) {
    // if there's argument
    argList = tokenize(argv);
    // tokenize argv. (asutil.hpp)
    if (V) {
      cout << "List of arguments:" << endl;
      for (size_t j = 0; j < argList.size(); j++) {
        cout << argList.at(j) << endl;
      }
    }
  } else {
    if (V) {
      cout << "No arguments provided." << endl;
    }
  }
  
  if (V) {
    cout << "Check if it is internal command." << endl;
  }
  int built_in = is_built_in(file, argList);
  // call is_built_in. if built-in it will be proceed and return a succ/fail value, if not return 1
  if (built_in == 1) { 
    // if "file argv" is not a built-in command:
    if (V) {
      cout << "Not an internal command." << endl;
    }

    // convert (argv) vector<string> to char** for execvp
    char **args = new char*[argList.size()+2];
    args[0] = const_cast<char*>(file.c_str());
    size_t i;
    for (i = 0; i < argList.size(); i++) {
      args[i+1] = const_cast<char*>(argList.at(i).c_str());
    }
    args[i+1] = NULL;
    // mark args end

    if (V) {
      cout << "================== EXECUTE START ==================" << endl;
    }

    cout << color() << flush;
    // reset output color
    int file = 0;
    int stdout = 0;
    if (rdfile != "") {
      // if redirect to file, try to avoid read only.
      file = open(rdfile.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      stdout = dup(1);
      // save stdout;
      dup2(file, 1);
    }
    
    int success;
    int status;
    pid_t c_pid, pid;
    c_pid = fork();
    if (c_pid == 0) {
      //child process running
      execvp(args[0], args);
      if (rdfile != "") {
        // if redirected output, set it back to stdout
        dup2(stdout, 1);
      }
      perror("exec failed");
      if (V) {
        cout << color("green") << flush;
        cout <<"=================== EXECUTE END ===================" << endl;
      }
      delete[] args;
      exit(1);
    }
    else if (c_pid > 0) {
      if ((pid = wait(&status)) < 0) {
        perror("waiting");
        if (rdfile != "") {
          // if redirected output, set it back to stdout
          dup2(stdout, 1);
        }
        if (V) {
          cout << color("green") << flush;
          cout <<"=================== EXECUTE END ===================" << endl;
        }
      }
      if (WIFEXITED(status)) {
        success = WEXITSTATUS(status);
      }
      if (success == 0) {
        if (rdfile != "") {
          // if redirected output, set it back to stdout
          dup2(stdout, 1);
        }
        if (V) {
          cout << color("green") << flush;
          cout <<"=================== EXECUTE END ===================" << endl;
        }
        delete[] args;
        return true;
      }
      
      if (rdfile != "") {
        // if redirected output, set it back to stdout
        dup2(stdout, 1);
      }
      
      if (V) {
        cout << color("green") << flush;
        cout <<"=================== EXECUTE END ===================" << endl;
      }
      
      delete[] args;
      return false;
    }
    else {
      perror("fork failed");
      if (rdfile != "") {
        // if redirected output, set it back to stdout
        dup2(stdout, 1);
      }

      if (V) {
        cout << color("green") << flush;
        cout <<"=================== EXECUTE END ===================" << endl;
      }
      
      delete[] args;
      exit(1);
    }

    delete[] args;
  } else if (built_in == 0) {
    // is built-in cmd and success
    return true;
  } else {
    // is built-in cmd but failed
    return false;
  }
  return true;
}













bool execCommand(vector<s_cmd> cmdList) {
  // all the connectors that are allowed to exist before a (
  vector<string> connectors;
  connectors.push_back("&&");
  connectors.push_back("||");
  connectors.push_back(";");
  
  if (V) {
    cout << "=======generateExecCommand========" << endl;
  }
  // flags to check if we need to run current command based on previous command
  bool previousStatus = true;
  bool runCurrentCommand = true;
  for (size_t i = 0; i < cmdList.size(); i++) {
    if (V) {
      cout << "We have " << cmdList.size() << " commands, now executing command " << i+1 << endl;
      cout << printlist(cmdList, i);
      cout << "First we check the exec bit: " << cmdList.at(i).exec << endl;
    }

    int parenthesis_count = 0;
    // construct recursive function based on ()s
    if (cmdList.at(i).exec == "(") {
      if (V) {
        cout << "Checking previous command exec" << endl;
      }
      if (i != 0) {
        if ((in_vector(cmdList.at(i-1).exec, connectors)) && (cmdList.at(i-1).file != "")) {
          // if the previous command has a valid connector but it is not empty.
          // i.e.: "&& echo A ( echo B".
          // SYNTAX ERROR
          cout << color("red") << flush;
          cout << "Syntax error." << flush;
          cout << color("green") << endl;
          return false;
        }
      }
      if (V) {
        cout << "Oooops we found a ( operator\nPush all commands before corresponding ) into a new cmdList and recurse execCommand." << endl;
      }
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
          if (V) cout << "Checking this command file" << endl;
          if (cmdList.at(i).file != "") {
            // if the ) line has command
            // i.e.: "echo B ) echo A"
            // SYNTAX ERROR
            cout << color("red") << flush;
            cout << "Syntax error." << flush;
            cout << color("green") << endl;
            return false;
          }
          if (V) {
            cout << printlist(cmdList, i);
          }
          parenthesis_count--;
          s_cmd child_cmd(cmdList.at(i).exec, cmdList.at(i).file, cmdList.at(i).argv);
          child_cmdList.push_back(child_cmd);
          
          if (V) {
            cout << "Pushed in." << endl;
            cout << printlist(child_cmdList);
            cout << "Check parenthesis_count to see if this is the corresponding parenthesis: " << parenthesis_count << endl;
          }

          if (parenthesis_count >= 0) {
            if (V) {
              cout << "No it is not the corresponding parenthesis. Keep pushing." << endl;
            }
          } else {
            if (V) {
              cout << "Yes it is the corresponding parenthesis. Prepare for executing." << endl;
              cout << "Check runCurrentCommand to see if we need to run this part: " << runCurrentCommand << endl;
            }
            if (runCurrentCommand) {
              if (V) {
                cout << "Ok, enter recursive function." << endl;
              }
              previousStatus = execCommand(child_cmdList);
            } else {
              if (V) {
                cout << "No, we don't need to run this part. Ignore and Next." << endl;
              }
            }
            break;
          }
        }
      }


    } else if (cmdList.at(i).exec == ";") {
      if (V) {
        cout << "Okay, run anyway." << endl;
      }
      runCurrentCommand = true;
    }

    else if (cmdList.at(i).exec == "&&") {
      if (V) {
        cout << "Let's see if our previous command executed successfully: ";
      }
      if (!previousStatus) {
        if (V) {
          cout << "false. Do not execute next command." << endl;
        }
        runCurrentCommand = false;
      } else {
        if (V) {
          cout << "true. Execute next command." << endl;
        }
        runCurrentCommand = true;
      }
    }

    else if (cmdList.at(i).exec == "||") {
      if (V) {
        cout << "Let's see if our previous command executed successfully: ";
      }
      if (!previousStatus) {
        if (V) {
          cout << "false. Execute next command." << endl;
        }
        runCurrentCommand = true;
      } else {
        if (V) {
          cout << "true. Do not execute next command." << endl;
        }
        runCurrentCommand = false;
      }
    }

    else if (cmdList.at(i).exec == ">|") {
      if (V) {
        cout << "Filepipe." << endl;
      }
      cmdList.at(i).argv += " /tmp/pipefile.tmp";
      // pass in pipefile as an argument
      // use /tmp to avoid read only
      if (V) {
        cout << "Let's see if our previous command executed successfully: ";
      }
      if (!previousStatus) {
        if (V) {
          cout << "false. Do not execute next command." << endl;
        }
        runCurrentCommand = false;
      } else {
        if (V) {
          cout << "true. Execute next command." << endl;
        }
        runCurrentCommand = true;
      }
    }

    else if (cmdList.at(i).exec == ">") {
      if (V) {
        cout << "Handled in the prev command as redirect output. Skip" << endl;
      }
      runCurrentCommand = false;
    }




    try {
      // unmatched parenthesis will make error here. try catch is a temp fix. waiting for revisit
      if (cmdList.at(i).file == "") {
        if (V) {
          cout << "Empty line. Ignored. Next." << endl;
        }
      } else if (runCurrentCommand) {
        if (V) {
          cout << "So now we decided to execute next command." << endl;
        }

        string rdfile = "";
        if (i != (cmdList.size()-1)) {
          if (V) {
            cout << "Check the next command's exec bit to see if we need to do stdout redirect job: " << cmdList.at(i+1).exec << endl;
          }
          if (cmdList.at(i+1).exec == ">|") {
            if (V) {
              cout << "Alright, file piping requested. Setting filepipe = true; rdfile = /tmp/pipefile.tmp;" << endl;
            }
            rdfile = "/tmp/pipefile.tmp";
          } else if (cmdList.at(i+1).exec == ">") {
            if (V) {
              cout << "Alright, stdout redirect requested. Setting rdfile = " << cmdList.at(i+1).file << endl;
            }
            if (cmdList.at(i+1).file == "") {
              cout << color("red") << flush;
              cout << "Syntax error: > no redirect path provided. Will output to stdout." << endl;
              cout << color("green") << flush;
            }
            rdfile = cmdList.at(i+1).file;
          }
        } else {
          if (V) {
            cout << "This is already the last command, no need to check for piping exec bit." << endl;
          }
        }
        
        // EXECUTE
        previousStatus = EXECUTE(cmdList.at(i).file, cmdList.at(i).argv, rdfile);

        if (cmdList.at(i).exec == ">|") {
          // /tmp/pipefile.tmp already used, remove it
          remove("/tmp/pipefile.tmp"); // clean up pipefile.tmp
        }

        if (V) {
          cout << "Command " << i << " executed. isSuccess? " << previousStatus << endl;
        }
      }
    } catch (std::exception const& e) {
      cout << color("red") << flush;
      cout << "Syntax error" << endl;
      cout << color("green") << flush;
    }
  }


  if (V) {
    cout << "Exiting function execCommand()" << endl;
  }

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

void printprompt() {
  // to distinguish with system shell I use "[rShell<#>]$" instead of "$"
  cout << color("green", "bold") << flush;
  cout << "" << user << "@" << host << flush;
  cout << color() << flush;
  cout << ":" << flush;
  cout << color("cyan", "bold") << flush;
  cout << dir << flush;
  cout << color() << flush;
  cout << ":" << flush;
  cout << color("yellow") << flush;
  cout << "[rShell<" << cmdHistoryPos << ">]" << flush;
  cout << color() << flush;
  cout << "$ " << flush; 
  if (V) cout << color("green") << flush;
}










string getCmd() {
  // GET USER INPUT // replace getline
  string cmdBuffer = "";
  size_t cursor = 0;
  char inchar = 0;
  bool enter = false;
  while (!enter) {
    inchar = getkey();
    // getkey (asutil.hpp)
    if (inchar == KEY_BKSP) { // 127 = backspace
      if (V) {
        cout << "Backspace detected." << endl;
      }
      if (cmdBuffer.length() != 0) {
        // erase everything <cmdBuffer> on screen
        for (size_t i = cursor; i < cmdBuffer.length(); i++) {
          cout << " " << flush;
        }
        for (size_t i = 0; i < cmdBuffer.length(); i++) {
          cout << "\b \b" << flush;
        }
        cursor--;
        if (V) {
          cout << "Erasing..." << endl;
          cout << str_pos(cmdBuffer, cursor);
        }
        cmdBuffer.erase(cmdBuffer.begin()+cursor);
        if (V) {
          cout << "Erased." << endl;
          cout << str_pos(cmdBuffer, cursor);
        }
        cout << color() << flush;
        cout << cmdBuffer << flush;
        if (V) {
          cout << color("green") << flush;
        }

        // fix onscreen cursor position
        for (size_t i = cmdBuffer.length(); i > cursor; i--) {
          cout << "\b" << flush;
        }
      }
    }










    else if (inchar == KEY_TAB) {
      if (V) {
        cout << endl << "TAB detected." << endl;
      }
      
      if (V) {
        cout << "Getting filenameBuffer.." << endl;
      }
      string filenameBuffer = "";
      for (size_t i = 0; i < cmdBuffer.length(); i++) {
        if (cmdBuffer.at(cmdBuffer.length()-1-i) == ' ') {
          filenameBuffer = cmdBuffer.substr(cmdBuffer.length()-i);
          break;
        }
      }
      if (V) {
        cout << "filenameBuffer: " << filenameBuffer << endl;
      }


      if (V) {
        cout << "Getting file list..." << endl;
        cout << "EXECUTE(\"ls\", \"\", \"/tmp/pathlist.tmp\");" << endl;
      }
      remove("/tmp/pathlist.tmp");
      EXECUTE("ls", "", "/tmp/pathlist.tmp");
      // list files in CWD and redirect output to a file for scanning
      fstream file;
      file.open("/tmp/pathlist.tmp");
      vector<string> pathlist;
      string pathname;
      while(file >> pathname) {
        pathlist.push_back(pathname);
      }
      if (V) {
        cout << "Pathlist: " << endl << print_v(pathlist);
      }

      vector<string> candidates;
      if (filenameBuffer == "") {

        // if user entered nothing that can be considered as a path
        if (V) {
          cout << "Filename empty, cout everything in Pathlist.." << endl;
        }
        cout << color() << flush;
        if (!V) {
          cout << endl;
        }
        for (size_t i = 0; i < pathlist.size(); i++) {
          cout << pathlist[i] << "\t";
        }
        cout << endl;
        if (!V) {
          printprompt();
        }
        cout << cmdBuffer;
        cout << color("green") << flush;

      } else {

        // if user entered something as a path
        if (V) {
          cout << "Filename not empty, comparing Pathlist.." << endl;
        }
        candidates = pathlist;
        for(size_t i = 0; i < filenameBuffer.length(); i++) {
        // scan through filenameBuffer;
          for(size_t j = 0; j < candidates.size(); j++) {
            // scan through a single candidate
            if (V) {
              cout << "Compare filenameBuffer[" << i << "]: " << endl;
              cout << str_pos(filenameBuffer, i);
              cout << "With pathlist[" << j << "][" << i << "]" << endl;
              cout << str_pos(candidates.at(j), i);
            }
            if (filenameBuffer.at(i) == candidates.at(j).at(i)) {
              if (V) {
                cout << "Match. Do nothing." << endl;
              }
            } else {
              if (V) {
                cout << "Does not match, erase from candidates." << endl;
              }
              candidates.erase(candidates.begin()+j);
              j--;
              if (V) {
                cout << "Candidates: " << endl << print_v(candidates);
              }
            }
          }
        }
        if (V) {
          cout << "After selecting candidates: " << endl << print_v(candidates) << endl;
        }
      }
      /*
      cout << color() << flush;
      if (candidates.size() > 1) { // more than 1 candidates
        cout << endl;
        for(size_t i = 0; i < candidates.size(); i++) {
          vector<string> testargv; 
          testargv.push_back("-d");
          testargv.push_back(candidates.at(i));
          cout << color() << flush;
          cout << "  " << candidates.at(i);
          cout << color("green") << flush;
          if (test(testargv) == 0) { // check if candidate is a directory
            cout << color() << flush;
            cout << "/";
          }
        }
        cout << endl;
        if (!V) {
          printprompt();
        }
        cout << color() << flush;
        cout << cmdBuffer << flush;
      } else if (candidates.size() == 1) {
        // only 1 candidate
        candidates.at(0) = candidates.at(0).substr(filenameBuffer.length());
        if (V) {
          printprompt();
          cout << color() << flush;
          cout << cmdBuffer;
        }
        cout << candidates.at(0);
        cmdBuffer += candidates.at(0);
        cursor = cmdBuffer.length();
      } else {
        // no candidate
        if (V) {
          cout << color() << flush;
          cout << cmdBuffer;
        }
      }
      if (V) {
        cout << color("green") << flush;
      }
      */
    }









    else if ((inchar == KEY_LF)||(inchar == KEY_CR)) { // if ENTER/RETURN
      cout << endl;
      enter = true;
      cmdHistoryPos = cmdHistory.size();
      cmdHistory.pop_back();
      if (cmdBuffer == ""){
        // if empty line
        if (V) {
          cout << "Empty Line." << endl;
        }
        return "";
      } 
      cmdHistory.push_back(cmdBuffer);
    }


    else if (inchar == 0x1b) {
      // 0x1b = ESC. 
      for (int ESC_IGNORE = 2; ESC_IGNORE > 0; ESC_IGNORE--) {
        inchar = getkey();
      }


      if (inchar == 'A') {
        // A up, 
        if (V) {
          cout << endl << "ANSI_ESCAPE_UP detected, looking into cmdHistory." << endl;
        }
        size_t sz = cmdHistory.size();
        if (cmdHistoryPos == sz-1) {
          cmdHistory.at(cmdHistoryPos) = cmdBuffer;
        }
        if (cmdHistoryPos != 0) {
          for (size_t i = 0; i < cmdBuffer.length(); i++) {
            cout << "\b \b" << flush;
          }
          cmdHistoryPos--;
          cmdBuffer = cmdHistory.at(cmdHistoryPos);
          if (V) {
            cout << "cmdHistory:" << endl << print_v(cmdHistory, cmdHistoryPos);
          }
          cout << color() << flush;
          cout << cmdBuffer << flush;
          cursor = cmdBuffer.length();
          if (V) {
            cout << color("green") << flush;
          }
        } else {
          if (V) {
            cout << cmdBuffer << flush;
          }
        }
      }

      else if (inchar == 'B') {
        // B down,
        if (V) {
          cout << endl << "ANSI_ESCAPE_DOWN detected, looking into cmdHistory." << endl;
        }
        if (cmdHistoryPos != cmdHistory.size()-1) {
          for (size_t i = 0; i < cmdBuffer.length(); i++) {
            cout << "\b \b" << flush;
          }
          cmdHistoryPos++;
          cmdBuffer = cmdHistory.at(cmdHistoryPos);
          if (V) {
            cout << "cmdHistory:" << endl << print_v(cmdHistory, cmdHistoryPos);
          }
          cout << color() << flush;
          cout << cmdBuffer << flush;
          cursor = cmdBuffer.length();
          if (V) {
            cout << color("green") << flush;
          }
        } else {
          if (V) {
            cout << cmdBuffer << flush;
          }
        }
      }

      else if (inchar == 'C') {
        // C right
        if (V) {
          cout << endl << "ANSI_ESCAPE_RIGHT detected." << endl;
        }
        if (cursor < cmdBuffer.size()) {
          if (V) {
            cout << str_pos(cmdBuffer, cursor);
            cout << color() << flush;
            cout << cmdBuffer << flush;
            for(size_t pos = cursor+1; pos < cmdBuffer.size(); pos++) {
              cout << "\b";
            }
            cout << color("green") << flush;
          } else {
            cout << color() << flush;
            cout << cmdBuffer.at(cursor);
            if (V) {
              cout << color("green") << flush;
            }
          }
          cursor++;
        }
      }

      else if (inchar == 'D') {
        // D left
        if (V) {
          cout << endl << "ANSI_ESCAPE_LEFT detected." << endl;
        }
        if (cursor != 0) {
          cout << "\b";
          //Cursor moves 1 position backwards
          cursor--;
          if (V) {
            cout << str_pos(cmdBuffer, cursor);
            cout << color() << flush;
            cout << cmdBuffer << flush;
            for(size_t pos = cursor; pos < cmdBuffer.size(); pos++) {
              cout << "\b";
            }
            cout << color("green") << flush;
          }
        }
      }


    } else {
      // If not special characters
      if (V) {
        cout << endl << str_pos(cmdBuffer, cursor);
      }
      cout << color() << flush;
      if (cursor < cmdBuffer.size()) {
        // if not at the end; need to move cursor and reprint
        string chr(1, inchar);
        cmdBuffer.insert(cursor, chr);
        if (V) {
          cout << cmdBuffer << flush;
        } else {
          for(size_t pos = cursor; pos < cmdBuffer.size(); pos++) {
            cout << cmdBuffer.at(pos);
          }
        }
        for(size_t pos = cursor; pos < cmdBuffer.size()-1; pos++) {
          cout << "\b";
        }
      } else {
        // if at the end; no need to move cursor or reprint
        cmdBuffer += inchar;
        if (V) {
          cout << cmdBuffer << flush;
        } else {
          cout << inchar << flush;
        }
      }
      if (V) {
        cout << color("green") << flush;
      }
      cursor++;
    }
    
  }
  return cmdBuffer;
}











int newCmd() {
  if (V) {
    cout << color("green") << flush;
  }
  string cmdBuffer = "";
  cmdHistory.push_back(cmdBuffer);
  printprompt();
  
  cmdBuffer = getCmd();
  
  // getline(cin, cmdBuffer);
  if (cmdBuffer == ""){
    // if (V) cout << "Quotation Error." << endl;
    return 0;// if quotation error
  } 
  // override for testing
  bool test = false;
  if (cmdBuffer == "test1") {
    // but as long as there's no multiple \"s in a quote, it will function well.
    cmdBuffer = "echo \"/bin/bash\" \"/bin/bash# This Part is a\\ # inside a quote\"; echo \"Hello World\" #This is a real quote ";
    test = true;
    //cmdBuffer = "cp \"/bin/bash\" \"/bin/bash# This Part is a\\\" # inside a quote\"; echo \"Hello World\" #This is a real quote ";
  } else if (cmdBuffer == "test2") {
    cmdBuffer = "ls -a; echo hallo welt && mkdir test || echo world; git status; exit; ls -l";
    test = true;
  } else if (cmdBuffer == "test3") {
    cmdBuffer = "ls & ls && ls";
    test = true;
  } else if (cmdBuffer == "test4") {
    cmdBuffer = "echo \"Hello && echo World\" && ls";
    test = true;
  } else if (cmdBuffer == "test5") {
    cmdBuffer = "(echo A && echo B) || (echo C && echo D)";
    test = true;
  }
  if (test) { 
    cout << color("yellow", "b") << "Test stdin override << ";
    cout << color("yellow", "r") << cmdBuffer << endl;
    cout << color() << flush;
  }

  if (V) {
    cout << color("green") << flush;
  }
  cmdBuffer = removeComment(cmdBuffer);
  if (cmdBuffer == ""){
    return 0;// if quotation error
  } 
  vector<s_cmd> cmdList;
  s_cmd s_cmd1(";", "", cmdBuffer);
  cmdList.push_back(s_cmd1);
  cmdList = parseCmd(cmdList, "(");
  cmdList = parseCmd(cmdList, ")");
  cmdList = parseCmd(cmdList, ";");
  cmdList = parseCmd(cmdList, "&&");
  cmdList = parseCmd(cmdList, "||");
  cmdList = parseCmd(cmdList, ">|");
  cmdList = parseCmd(cmdList, ">");
  cmdList = f_parenthesis(cmdList);
  cmdList = trimCmd(cmdList);
  execCommand(cmdList);
  return 0;
}








int main(int argc, char *argv[]) {
  // cmdline argument to toggle verbose output
  if (argc == 2) {
    string flag = argv[1];
    if (flag == "-v") {
      V = true;
    }
  }
  cout << color("cyan", "b");
  cout << "\n\nrShell [Version " << version << "]" << endl;
  cout << color("yellow");
  // cout << "rShell supports all commands in /bin/ and /usr/bin/ ." << endl;
  // cout << "rShell supports ./executable to execute file in current working directory." << endl;
  cout << "rShell supports absolute and relative path to executable." << endl;
  cout << color("yellow", "b")  << "rShell built-in commands:" << color("yellow")  << endl;
  cout << "  cd [PATH]                           # Change working directory." << endl;
  cout << "  exit [0-9 (optional)]               # Exit rShell <with optional exit code>." << endl;
  cout << "  test [-e|-f|-d (optional)] [PATH]   # Test file." << endl;
  cout << "  verbose [on|off]                    # Toggle verbose output." << endl;
  cout << "  viewcmdhistory                      # View command history."  << endl;
  cout << "  [ [-e|-f|-d (optional)] [PATH] ]    # Test file." << endl;
  cout << "  UP/DOWN arrow key                   # Navigate through command history." << endl;
  cout << "  LEFT/RIGHT arrow key                # Navigate in the cmdBuffer." << endl;
  cout << "  BACKSPACE                           # Took me quite a while to make this work." << endl;
  cout << color("yellow", "b") << "rShell supports connectors:" << color("yellow") << endl;
  cout << "  ;                                   # Execute unconditionally." << endl;
  cout << "  &&                                  # Execute if the previous command succ." << endl;
  cout << "  ||                                  # Execute if the previous command fail." << endl;
  // cout << "  >|                                  # Filepipe the prev command's output to next command." << endl;
  // cout << "     ** NOT in the way Unix pipe does. rShell does this by redirecting prev command's stdout to a FILE and pass the FILE as the LAST ARGUMENT to the next command." << endl;
  cout << "  > [PATH]                            # Redirect stdout to a file." << endl;
  cout << endl;
  user = getlogin();
  gethostname(host, 999);
  getcwd(chardir,BUFSIZ);
  dir = chardir;
  home = charh;
  dir = str_swap(dir, home, "~");
  while (1) {
    newCmd();
  }
  return 0;
}
