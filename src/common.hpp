#ifndef _COMMON_
#define _COMMON_

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

std::string version = "20151110 Test command & Precedence Operators";
bool V = true;
std::string user = "user";
char host[999] = "hostname";
char chardir[BUFSIZ];
char *charh = getenv("HOME");
std::string dir;
std::string home;
    

#endif
