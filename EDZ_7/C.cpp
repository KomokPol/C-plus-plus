#include <vector>
#include <iterator>
#include <algorithm>
#include <bits/stdc++.h>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  int x = 0;
  std::cin >> x;

  std::cout << count(arr.begin(), arr.end(), x) << "\n";
}