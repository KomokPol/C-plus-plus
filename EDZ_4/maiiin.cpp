#include <cassert>
#include <cstring>
#include <iostream>
#include "cstring.h"
int main() {
  const char* a = "";
  const char* b = "aabc";
  const char* c = "dddaabdddabcaabcd";
  const char* d = "zxcbav";
  const char* e = "aab";
  const char* f = "aaab";
  const char* g = "abcabcabcd";
  const char* h = "abcabcd";
  std::cout << (Cstring::Strstr(a, a) == std::strstr(a, a));
  std::cout << (Cstring::Strstr(b, b) == std::strstr(b, b));
  std::cout << (Cstring::Strstr(b, a) == std::strstr(b, a));
  std::cout << (Cstring::Strstr(a, b) == std::strstr(a, b));
  std::cout << (Cstring::Strstr(b, b) == std::strstr(b, b));
  std::cout << (Cstring::Strstr(c, b) == std::strstr(c, b));
  std::cout << (Cstring::Strstr(d, b) == std::strstr(d, b));
  std::cout << (Cstring::Strstr(f, e) == std::strstr(f, e));
  std::cout << (Cstring::Strstr(g, h) == std::strstr(g, h));
}