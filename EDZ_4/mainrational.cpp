#include "rational.h"
#include <iostream>

int main() {
  Rational x = 5;        // дробь 5/1
  std::cout << x;        // вывод: 5
  x.SetDenominator(15);  // дробь 1/3 (5/15 сократилась)
  std::cout << x;        // вывод: 1/3
  std::cin >> x;         // ввод: 14/-6, дробь: -7/3 (общий множитель 2; знаменатель > 0)
}