#ifndef _BUILTIN_
#define _BUILTIN_

#include <iostream>

using namespace std;

int test(vector<string> argv) { // -1 on fail || 0 on succ
  // if (V) cout << color("green") << flush;
  if (argv.size() == 0) { return -1; }
  int result = -1;
  if (argv[0] == "-f") { // -f flag
    if (V) cout << "Check File" << endl;
    struct stat sb;
    stat(const_cast<char*>(argv[1].c_str()), &sb);
    if ((sb.st_mode & S_IFMT) == S_IFREG) {
      if (V) cout << "Input is a File. Filename entered: " << argv[1] << endl;
      result = 0;
    } else {
      if (V) cout << "Input is not a File. Filename entered: " << argv[1] << endl;
      result = -1;
    }
  } else if (argv[0] == "-d") { // -d flag
    if (V) cout << "Check Dir" << endl;
    struct stat sb;
    stat(const_cast<char*>(argv[1].c_str()), &sb);
    if ((sb.st_mode & S_IFMT) == S_IFDIR) {
      if (V) cout << "Input is a Directory. Filename entered: " << argv[1] << endl;
      result = 0;
    } else {
      if (V) cout << "Input is not a Directory. Filename entered: " << argv[1] << endl;
      result = -1;
    }
  } else if (argv[0] == "-e") { // -e flag
    if (V) cout << "Check Exist" << endl;
    struct stat sb;
    if( stat(const_cast<char*>(argv[1].c_str()), &sb) != 0) {
      if (V) cout << "Input does not exist. Filename entered: " << argv[1] << endl;
      result = -1;
    } else {
      if (V) cout << "Input exists. Filename entered: " << argv[1] << endl;
      result = 0;
    }
  } else if (argv[0].at(0) == '-') { // if argv is a flag but not recognized
    cout << color() << "Invalid flag: " << argv[0] << color("green") << endl;
  } else { // if no flag then add -e as default flag and recurse to execute
    argv.insert(argv.begin(), "-e");
    if (test(argv)) {
      result = 0;
    } else {
      result = -1;
    }
  }
  // if (V) cout << color() << flush;
  return result;
}







int verbose(vector<string> argv) { // -1 on fail || 0 on succ
  if (argv.size() == 0) {
    cout << color() << flush;
    cout << "Verbose output control. Valid options: [on|off]" << endl;
    return -1;
  } else if ((argv.at(0) == "on") || argv.at(0) == "1") {
    V = true;
    cout << color() << flush;
    cout << "Verbose output is now turned on.\nToggle: verbose [on|off]" << endl;
    cout << color("green") << flush;
  } else {
    V = false;
    cout << color() << flush;
    cout << "Verbose output is now turned off.\nToggle: verbose [on|off]" << endl;
    cout << color("green") << flush;
  }
  return 0;
}





int terminate(vector<string> argv) { // the only thing can happen is exit(0) so must return 0; // wait is there even a return?
  cout << color() << flush;
  if (argv.size() != 0) {
    int exitcode = atoi(argv[0].c_str());
    cout << "Exit with code " << exitcode << endl;
    exit(exitcode);
  } else {
    cout << "Exit" << endl;
    exit(0);
  }
  return 0; // not necessary
}



int cd(vector<string> argv) {
  if (argv.size() == 0) {
    // cout << color() << flush;
    // cout << dir << endl;
    // cout << color("green") << flush;
    argv.push_back("~");
  }
  /*else if*/ {
    argv[0] = str_swap(argv[0], home, "~");
    if (chdir(argv[0].c_str())) {
      cout << color() << flush;
      cout << "chdir: change directory failed." << endl;
      cout << color("green") << flush;
      return -1;
    } else {
      getcwd(chardir,BUFSIZ);
      dir = chardir;
      dir = str_swap(dir, home, "~");
    }
  }
  return 0;
}


int is_built_in(string file, vector<string> argv) {
  if (V) cout << "Processing by is_built_in()" << endl;
  // if "file argv" is a built in command then execute it and {return 0 if success, -1 if failed}, otherwise do nothing and return 1
  int exitcode = 1;
  // handle rshell built-in commands;


  if (file == "exit") {
    exitcode = 0; // mark as internal command.
    return terminate(argv);
  }

  if (file == "verbose") {
    exitcode = -1; // mark as internal command.
    return verbose(argv);
  }

  else if (file == "test") {
    exitcode = -1; // mark as internal command.
    return test(argv);
  }

  else if (file == "[") {
    if (argv.at(argv.size()-1) == "]") {
      exitcode = -1; // mark as internal command.
      if (V) cout << "[] detected, identified as alias to built-in cmd `test`." << endl << "argv.pop_back to erase ] and pass it to test function." << endl;
      argv.pop_back();
      return test(argv);
    }
  }

  else if (file == "cd") {
    exitcode = -1; // mark as internal command.
    return cd(argv);
  }

  else if (file == "viewcmdhistory") {
    exitcode = 0;
    cout << color();
    cout << print_v(cmdHistory, cmdHistoryPos-1);
    cout << color("green");
    return 0;
  }

  return exitcode;
}



#endif
