#include <iostream>
#include <map>
#include <string>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::map<std::string, std::map<std::string, int64_t>> map;

  std::string buyer;
  std::string product;
  int64_t count = 0;

  while (std::cin >> buyer >> product >> count) {
    map[buyer][product] += count;
  }

  for (const auto &person : map) {
    std::cout << person.first << ":\n";
    for (const auto &product : person.second) {
      std::cout << product.first << " " << product.second << "\n";
    }
  }
};