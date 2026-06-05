#include "copy.h"
#include <iostream>

int main() {
  int64_t a[] = {1, 2, 3, 4};
  int32_t b[] = {5, 6, 7};
  Copy(b, b + 2, a + 1);
  std::cout << "Copy: [";
  for (size_t i = 0; i < 4; ++i) {
    std::cout << a[i] << (i < 3 ? ", " : "");
  }
  std::cout << "]\n";
  CopyBackward(b + 1, b + 3, a + 2);
  std::cout << "CopyBackward: [";
  for (size_t i = 0; i < 4; ++i) {
    std::cout << a[i] << (i < 3 ? ", " : "");
  }
  std::cout << "]\n";
}