#include <iostream>
#include <unordered_map>
#include <string>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int pairs = 0;
  std::cin >> pairs;

  std::unordered_map<std::string, std::string> dict;
  for (int i = 0; i < pairs; i++) {
    std::string word1;
    std::string word2;
    std::cin >> word1 >> word2;
    dict[word1] = word2;
    dict[word2] = word1;
  }

  int count = 0;
  std::cin >> count;
  for (int i = 0; i < count; i++) {
    std::string query;
    std::cin >> query;
    std::cout << dict[query] << "\n";
  }
};