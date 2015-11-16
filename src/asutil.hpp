// Author: Angda Song
// Email: songangda@gmail.com
// Version: 11/14/2015 10:48PM
// Build for personal use. May be used in future projects.
// Functions:
//   int in_vector(string, vector<string>);
//   int in_vector(int, vector<int>);
//   string print_v(vector<T>, bool = true);
//   string color(string = "reset", string = "r", string = "f");
//   string str_pos(string, size_t);
//   vector<string> tokenize(string);
//   string str_swap(string, string, string);
//   char getkey();
// Variables:
//   char KEY_LF;
//   char KEY_CR;


#ifndef _A_S_UTIL_
#define _A_S_UTIL_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <termios.h>

using namespace std;

int in_vector(string x, vector<string> vec) {
  for (size_t i = 0; i < vec.size(); i++) {
    if (vec[i] == x) {
      return i;
    }
  }
  return -1;
}
int in_vector(int x, vector<int> vec) {
  for (size_t i = 0; i < vec.size(); i++) {
    if (vec[i] == x) {
      return i;
    }
  }
  return -1;
}

template<typename T>
string print_v(vector<T> vec, size_t pos = SIZE_MAX, bool printEmpty = true) {
  ostringstream oss;
  if (pos == SIZE_MAX) {
    for (size_t i = 0; i < vec.size(); i++) {
      if ((vec[i] != "") || printEmpty) {
        oss << i << "\t[" << vec[i] << "]" << endl;
      }
    }
  } else {
    if (pos >= vec.size()) {
      oss << "Pos " << pos << " out of range " << vec.size() << endl;
    }
    for (size_t i = 0; i < vec.size(); i++) {
      if ((vec[i] != "") || printEmpty) {
        oss << i;
        if (i==pos) {
          oss << " -->\t";
        }
        else {
          oss << "\t";
        }
        oss <<" [" << vec[i] << "]" << endl;
      }
    }
  }
  return oss.str();
}

string color(string color = "reset", string bold = "r", string ground = "f") {
  ostringstream oss;
  int index;
  if (color == "reset") {
    // reset foreground
    char f[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
    char b[] = { 0x1b, '[', '0', ';', '4', '9', 'm', 0 };
    oss << f << b << flush;
  } else {
    // color handle
    // c++11 required
/*
    vector<string> colorlist {
      "black", "red", "green", "yellow", "blue", "purple", "cyan", "white", "", "default"
    };
*/
    // c++98 fallback
    vector<string> colorlist;
    colorlist.push_back("black");
    colorlist.push_back("red");
    colorlist.push_back("green");
    colorlist.push_back("yellow");
    colorlist.push_back("blue");
    colorlist.push_back("purple");
    colorlist.push_back("cyan");
    colorlist.push_back("white");
    colorlist.push_back("");
    colorlist.push_back("default");


    
    index = in_vector(color, colorlist);
    if (index == -1) {
      oss << "color() error: Unknown color option: " << color << endl << "Valid options: " << endl;
      print_v(colorlist, false);
      exit(1);
    }
    char c = index +'0';
    // bold handle
    // c++11 required
/*
    vector<string> boldlist {
      "r", "b", "", "", "", "", "", "", "", "", "regular", "bold"
    };
*/
    // c++98 fallback
    vector<string> boldlist;
    boldlist.push_back("r");
    boldlist.push_back("b");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("");
    boldlist.push_back("regular");
    boldlist.push_back("bold");

    index = in_vector(bold, boldlist);
    while (index >= 10) { index -= 10; }
    if (index == -1) {
      oss << "color() error: Unknown bold option: " << bold << endl << "Valid options: " << endl;
      print_v(boldlist, false);
      exit(1);
    }
    char b = index +'0';


    // fore/background handle
    // c++11 required
/*
    vector<string> groundlist {
      "", "", "", "f", "b"
    };
*/ 
    // c++98 fallback
    vector<string> groundlist;
    groundlist.push_back("");
    groundlist.push_back("");
    groundlist.push_back("");
    groundlist.push_back("f");
    groundlist.push_back("b");

    index = in_vector(ground, groundlist);
    if (index == -1) {
      oss << "color() error: Unknown ground option: " << ground << endl << "Valid options: " << endl;
      print_v(groundlist, false);
      exit(1);
    }
    char g = index +'0';
    char colorcode[] = { 0x1b, '[', b, ';', g, c, 'm', 0 };
    oss << colorcode << flush;
  }
  return oss.str();
}

string str_pos(string str, size_t pos) {
  ostringstream oss;
  string blk = "-";
  size_t l = str.length();
  size_t w = l;
  if (pos > l) w = pos;
  if (pos >= l) {
    oss << color("red");
    for (size_t i = 0; i < w; i++) {
      oss << blk;
    }
    oss << endl << "pos (" << pos << ") out of range (" << l << ")" << endl;
    // oss << color();
  }
  for (size_t i = 0; i < w; i++) oss << blk;
  oss << endl << str;
  for (size_t i = l; i < w; i++) oss << " ";
  oss << endl;
  for (size_t i = 0; i < pos; i++) oss << blk;
  oss << "^";
  for (size_t i = pos+1; i < w; i++) oss << blk;
  oss << endl;
  return oss.str();
}

vector<string> tokenize(string str) {
  if (V) {cout << "====== Start Tokenize ======" << endl;}
  vector<string> token;
  bool isInQuote = false;
  for (size_t i = 0; i < str.length(); i++) { // tokenize str
  if (str.at(i) == '\"') {
    if (V) {cout << "\" found at position " << i << endl;}
    if (V) {cout << str_pos(str, i);}
    if ((i == 0) || (str.at(i-1) != '\\')) {
    if (V) {cout << "This is a real quotation mark." << endl;}
    str.erase(str.begin() + i);
    i--;
    isInQuote = !isInQuote;
    if (V) {cout << "[isInQuote]=" << isInQuote << endl;}
    }
    else {
      if (isInQuote) {
        if (V) {cout << "This is a slashed quotation mark in a quote." << endl;}
      }
    }
  } else if (str.at(i) == ' ') {
    if (V) {cout << str_pos(str, i);}
    if (!isInQuote) {
      if (V) {cout << "This is a real space. Tokenize." << endl;}
      if ((str.substr(0, i) != "") && (str.substr(0, i) != " ")) {
        token.push_back(str.substr(0, i));
      }
      str = str.substr(i+1);
      i=-1;
    } else {
      if (V) {cout << "This is a space in quote. Ignore." << endl;}
    }
  }
  }
  token.push_back(str);
  if (V) {
    for (size_t i = 0; i < token.size(); i++) {
      cout << "["<< token.at(i) << "] ";
    }
  }
  if (V) {cout << endl << "======= End Tokenize =======" << endl;}
  return token;
}




string str_swap(string str, string str_a, string str_b) {
  if (str.find(str_a) != string::npos) {
    str.replace(str.find(str_a), str_a.length(), str_b);
  } else if (str.find(str_b) != string::npos) {
    str.replace(str.find(str_b), str_b.length(), str_a);
  }
  return str;
}




int getkey() {
  int ch;
  struct termios t_old, t_new;
  tcgetattr(STDIN_FILENO, &t_old);
  t_new = t_old;
  t_new.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
  return ch;
}



char KEY_LF = 10;
char KEY_CR = 13;
// char KEY_UP[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };


#endif
