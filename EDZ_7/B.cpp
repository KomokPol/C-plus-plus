#include <bits/stdc++.h>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;
  std::unordered_map<std::string, int64_t> bank;

  while (n--) {
    int type = 0;
    std::cin >> type;
    std::string name;
    std::cin >> name;
    if (type == 1) {
      int64_t money = 0;
      std::cin >> money;
      bank[name] += money;
    } else if (type == 2) {
      auto it = bank.find(name);
      if (it == bank.end()) {
        std::cout << "ERROR\n";
      } else {
        std::cout << it->second << "\n";
      }
    }
  }
}





  copy_n(std::istream_iterator<int>(std::cin), n, arr.begin());