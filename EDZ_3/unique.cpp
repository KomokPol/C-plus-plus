#include "unique.h"
#include <iostream>

int main() {
  int a[]{1, 1, 2, 2, 3, 3, 4, 5, 5, 6};
  Unique(a, a + 10);  // возвращает a + 6 (a = [1, 2, 3, 4, 5, 6, ?, ?, ?])
  // ? означает, что элемент может быть произвольным

  std::cout << "Unique: ";
  for (int i = 0; i < 10; i++) {
    std::cout << a[i] << " ";
  }
  std::cout << "\n" << Unique(a, a + 10);
  std::cout << "\n" << a + 6 << "\n";
}