#ifndef _COMMON_
#define _COMMON_


#include <cstdlib>
#include <string>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <unistd.h>
#include <vector>
#include <stddef.h>
#include <vector>
#include <sys/wait.h>

using namespace std;

bool V = false;
// global verbose output control added.
// can be switched by "$ verbose [on|off]" in runtime

int version = 20151102;

char c_black[] = { 0x1b, '[', '0', ';', '3', '0', 'm', 0 };
char c_red[] = { 0x1b, '[', '0', ';', '3', '1', 'm', 0 };
char c_green[] = { 0x1b, '[', '0', ';', '3', '2', 'm', 0 };
char c_yellow[] = { 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
char c_blue[] = { 0x1b, '[', '0', ';', '3', '4', 'm', 0 };
char c_purple[] = { 0x1b, '[', '0', ';', '3', '5', 'm', 0 };
char c_cyan[] = { 0x1b, '[', '0', ';', '3', '6', 'm', 0 };
char c_white[] = { 0x1b, '[', '0', ';', '3', '7', 'm', 0 };
char c_reset[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };


#endif
