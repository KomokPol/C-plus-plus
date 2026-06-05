#include "isSortedUntil.h"
#include <iostream>

int main() {
  int a[]{1, 2, 2, 3, 3, 4, 2, 5, 6};
  std::cout << a << "\n" << a + 9 << "\n";
  std::cout << a + 6 << "\n";
  std::cout << IsSortedUntil(a, a + 9);
}