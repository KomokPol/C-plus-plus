#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

struct Team {
  int id;
  int points;
  int time;
};

struct Comparator {
  bool operator()(Team const& a, Team const& b) const {
    if (a.points != b.points) {
      return a.points > b.points;
    }
    if (a.time != b.time) {
      return a.time < b.time;
    }
    return a.id < b.id;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<Team> teams(n);
  for (int i = 0; i < n; ++i) {
    teams[i].id = i + 1;
    std::cin >> teams[i].points >> teams[i].time;
  }

  std::stable_sort(teams.begin(), teams.end(), Comparator());

  for (auto& team : teams) {
    std::cout << team.id << "\n";
  }
}