#include "reverse.h"
#include <iostream> 

int main() {
  int a[]{1, 2, 3, 4, 5};
  Reverse(a + 1, a + 4); // a == [1, 4, 3, 2, 5]
  std::cout << "Reverse: [";
  for (size_t i = 0; i < 5; ++i) {
    std::cout << a[i] << (i < 4 ? ", " : "");
  }
  std::cout << "]\n";
  Rotate(a, a + 2, a + 5); // a == [3, 2, 5, 1, 4]
  std::cout << "Reverse: [";
  for (size_t i = 0; i < 5; ++i) {
    std::cout << a[i] << (i < 4 ? ", " : "");
  }
  std::cout << "]\n";
}