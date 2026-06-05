#include <bits/stdc++.h>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int num_points = 0;
  std::cin >> num_points;
  int num_ops = 0;
  std::cin >> num_ops;

  std::vector<std::vector<int>> connections(num_points);

  while (num_ops--) {
    int type = 0;
    std::cin >> type;
    if (type == 1) {
      int u = 0;
      int v = 0;
      std::cin >> u >> v;
      --u;
      --v;
      connections[u].push_back(v);
      connections[v].push_back(u);
    } else if (type == 2) {
      int u = 0;
      std::cin >> u;
      --u;
      for (size_t i = 0; i < connections[u].size(); ++i) {
        std::cout << connections[u][i] + 1;
        if (i != connections[u].size() - 1) {
          std::cout << ' ';
        }
      }
      std::cout << '\n';
    }
  }
}