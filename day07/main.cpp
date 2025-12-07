#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

constexpr int WIDTH = 141;

std::size_t toUZ(std::integral auto x) { return static_cast<std::size_t>(x); }

template <typename T>
bool contains(const std::vector<T>& vec, const T& target) {
  return std::ranges::find(vec, target) != vec.end();
}

int part_1(int initial_x, int max_y,
           const std::array<std::vector<int>, WIDTH>& splitters) {
  std::set<int> beams{initial_x};
  int split_count_p1{0};
  int y{0};
  while (y < max_y) {
    std::vector<int> to_add;
    std::vector<int> to_remove;
    for (auto x : beams) {
      if (contains(splitters[static_cast<size_t>(x)], y)) {
        ++split_count_p1;
        to_remove.emplace_back(x);
        to_add.emplace_back(x - 1);
        to_add.emplace_back(x + 1);
      }
    }
    for (auto x : to_add) {
      beams.insert(x);
    }
    for (auto x : to_remove) {
      beams.erase(x);
    }
    ++y;
  }
  return split_count_p1;
}

long part_2(int initial_x, int max_y,
            const std::array<std::vector<int>, WIDTH>& splitters) {
  std::array<long, WIDTH> x_to_timeline_count{};

  x_to_timeline_count[toUZ(initial_x)] = 1;
  int y{0};
  while (y < max_y) {
    std::array<long, WIDTH> diffs{};
    for (int x{0}; x < WIDTH; ++x) {
      if (!x_to_timeline_count[toUZ(x)] || !contains(splitters[toUZ(x)], y))
        continue;
      diffs[toUZ(x - 1)] += x_to_timeline_count[toUZ(x)];
      diffs[toUZ(x + 1)] += x_to_timeline_count[toUZ(x)];
      diffs[toUZ(x)] = -x_to_timeline_count[toUZ(x)];
    }
    for (size_t x{0}; x < diffs.size(); ++x) {
      x_to_timeline_count[x] += diffs[x];
    }
    ++y;
  }
  return std::accumulate(x_to_timeline_count.begin(), x_to_timeline_count.end(),
                         0l);
}

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

int main(int argc, char** argv) {
  auto before_parsing = high_resolution_clock::now();
  if (argc < 2) {
    std::cout << "Give input file\n";
    return 0;
  }
  std::fstream file{argv[1], std::ios::in};
  if (!file) {
    std::cout << "File not found\n";
    return 0;
  }
  std::array<std::vector<int>, WIDTH> splitters{};
  std::string line;
  int initial_x;
  int max_y{-1};
  while (std::getline(file, line) && !line.empty()) {
    ++max_y;
    for (size_t i{0}; i < line.size(); ++i) {
      if (line[i] == 'S') {
        initial_x = static_cast<int>(i);
      }
      if (line[i] == '^')
        splitters[i].emplace_back(max_y);
    }
  }

  auto before_p1 = high_resolution_clock::now();
  std::cout << "Part 1: " << part_1(initial_x, max_y, splitters) << '\n';
  auto before_p2 = high_resolution_clock::now();
  std::cout << "Part 2: " << part_2(initial_x, max_y, splitters) << '\n';
  auto after = high_resolution_clock::now();
  std::cout << "Times:\n";
  std::cout << "Parsing: "
            << duration_cast<microseconds>(before_p1 - before_parsing) << '\n';
  std::cout << "Part  1: " << duration_cast<microseconds>(before_p2 - before_p1)
            << '\n';
  std::cout << "Part  2: " << duration_cast<microseconds>(after - before_p2)
            << '\n';
  std::cout << "Total  : "
            << duration_cast<microseconds>(after - before_parsing) << '\n';
}
