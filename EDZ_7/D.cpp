#include <bits/stdc++.h>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n = 0;
  std::cin >> n;
  std::vector<int> colors(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> colors[i];
  }
  int m = 0;
  std::cin >> m;
  while (m--) {
    int x = 0;
    std::cin >> x;
    auto range = equal_range(colors.begin(), colors.end(), x);
    std::cout << (range.second - range.first) << "\n";
  }
}