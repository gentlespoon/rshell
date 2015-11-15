#ifndef _COMMON_
#define _COMMON_

#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stddef.h>
#include <cstdio>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

string version = "20151110 Test command & Precedence Operators";
bool V = false;
string user = "user";
char host[999] = "hostname";
char chardir[BUFSIZ];
char *charh = getenv("HOME");
string dir;
string home;
vector<string> cmdHistory;
size_t cmdHistoryPos = 0;
    

#endif
