#ifndef _COMMON_
#define _COMMON_

#ifndef SIZE_MAX
#define SIZE_MAX 99999
#endif


#include <boost/algorithm/string.hpp>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

string version = "20151119 cmdHistory & Pipe";
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
