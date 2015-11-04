#ifndef VERBOSE
#define VERBOSE

#include "common.hpp"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

template <typename T> string vout(T const & x) {
  ostringstream oss;
  oss << c_green;
  if (!V) {}
  else oss << x;
  oss << c_reset;
  return oss.str();
}

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
    for (size_t i = 0; i < w+3; i++) oss << blk;
    oss << endl << vblk << str;
    for (size_t i = l; i < w+1; i++) oss << " ";
    oss << vblk << endl << vblk;
    for (size_t i = 0; i < loc; i++) oss << "_";
    oss << "^";
    for (size_t i = loc; i < w; i++) oss << "_";
    oss << vblk << endl;
    for (size_t i = 0; i < w+3; i++) oss << blk;
    oss << endl;
  }
  oss << c_reset;
  return oss.str();
}

#endif