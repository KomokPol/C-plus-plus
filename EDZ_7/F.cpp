#include <iostream>
#include <queue>
#include <iomanip>

int main() {
  int n = 0;
  std::cin >> n;
  std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> heap;
  for (int i = 0; i < n; i++) {
    int64_t x = 0;
    std::cin >> x;
    heap.push(x);
  }
  double result = 0.0;
  while (heap.size() > 1) {
    int64_t first = heap.top();
    heap.pop();
    int64_t second = heap.top();
    heap.pop();
    int64_t sum = first + second;
    result += static_cast<double>(sum) * 0.05;
    heap.push(sum);
  }
  std::cout << result;
};