#include <string>
#include <iostream>

using namespace std;
int main() {
  string trimmed = "This is a test.";
  int space = trimmed.find(' ');
  cout << trimmed.substr(0, space) << endl;
  cout << trimmed.substr(space+1) << endl;


}
