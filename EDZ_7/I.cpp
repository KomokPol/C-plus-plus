#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::map<std::string, int> counts;
  std::string line;

  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
      counts[word]++;
    }
  }
  std::vector<std::pair<std::string, int>> words;
  for (const auto& c : counts) {
    words.emplace_back(c.first, c.second);
  }
  std::sort(words.begin(), words.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    if (a.second == b.second) {
      return a.first < b.first;
    }
    return a.second > b.second;
  });

  for (const auto& w : words) {
    std::cout << w.first << "\n";
  }
};