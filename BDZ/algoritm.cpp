// int main() {
//   int count = 0;
//   std::cin >> count;
//   std::vector<int> arr(count);
//   for (int i = 0; i < count; ++i) {
//     int num = 0;
//     std::cin >> num;
//     arr[i] = num;
//   }
//   std::sort(arr.begin(), arr.end());
//   int result = 0;
//   int summa = 0;
//   for (int i = 0; i < count; ++i) {
//     summa += arr[i];
//     if (i != 0) {
//       result += summa;
//     }
//   }
//   std::cout << result;
// }

// int main() {
//   std::map<std::string, int> dict;
//   std::string str;
//   while (std::cin >> str) {
//     if (dict.find(str) == dict.end()) {
//       std::cout << "0 ";
//       dict[str] = 1;
//     } else {
//       std::cout << dict[str] << " ";
//       dict[str] += 1;
//     }
//   }
// }

// int main() {
//   std::map<std::string, int> dict;
//   std::string str;
//   while (std::cin >> str) {
//     if (dict.find(str) == dict.end()) {
//       dict[str] = 1;
//     } else {
//       dict[str] += 1;
//     }
//   }
//   std::vector<std::tuple<int, std::string>> arr;
//   for (const auto& el : dict) {
//     std::tuple<int, std::string> tup(-el.second, el.first);
//     arr.push_back(tup);
//   }
//   std::sort(arr.begin(), arr.end());
//   for (size_t i = 0; i < arr.size(); ++i) {
//     std::cout << std::get<1>(arr[i]) << "\n";
//   }
// }

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <string>

int main() {
  int count = 0;
  std::cin >> count;
  std::vector<std::string> arr(count);
  for (int i = 0; i < count; ++i) {
    std::string str;
    std::cin >> str;
    arr[i] = str;
  }
  std::sort(arr.begin(), arr.end(), [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
  int result = 0;
  for (size_t i = 0; i < arr.size() - 1; ++i) {
    for (size_t j = i + 1; j < arr.size(); ++j) {
      if (arr[i] == arr[j]) {
        result += 2;
      } else if (arr[j].find(arr[i]) != std::string::npos) {
        result += 1;
      }
    }
  }
  std::cout << result;
}