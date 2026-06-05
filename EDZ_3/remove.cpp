#include "remove.h"
#include <iostream>

int main() {
  int a[]{1, 2, 6, 4, 1, 1, 8, 0, 1};

  Remove(a, a + 9, 1);  // возвращает a + 5 (a = [2, 6, 4, 8, 0, ?, ?, ?, ?])
  // ? означает, что элемент может быть произвольным
  std::cout << "Remove: ";
  for (int i = 0; i < 9; i++) {
    std::cout << a[i] << " ";
  }
  std::cout << "\n" << Remove(a, a + 9, 1);
  std::cout << "\n" << a + 5 << "\n";
}