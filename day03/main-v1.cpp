#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

// Won't clean this :^)

int main(int argc, char** argv) {
  auto before = high_resolution_clock::now();
  if (argc < 2) {
    std::cout << "Give input file\n";
    return 0;
  }
  std::fstream file{argv[1], std::ios::in};
  if (!file) {
    std::cout << "File not found\n";
    return 0;
  }
  long p1_sum{0};
  long p2_sum{0};
  std::string line{};
  line.reserve(100);
  while (std::getline(file, line) && !line.empty()) {
    auto n = line.length();

    {
      std::array<size_t, 2> batt_line_idxs{n - 2, n - 1};
      for (int i{static_cast<int>(n) - 3}; i >= 0; --i) {
        size_t curr_line_idx = static_cast<size_t>(i);
        for (size_t batt_index{0}; batt_index < std::ssize(batt_line_idxs);
             ++batt_index) {
          auto batt_line_idx = batt_line_idxs[batt_index];
          if (line[curr_line_idx] >= line[batt_line_idx] &&
              curr_line_idx < batt_line_idx) {
            if (batt_index && (curr_line_idx) < batt_line_idxs[batt_index - 1])
              continue;
            std::swap(curr_line_idx, batt_line_idxs[batt_index]);
          }
        }
      }

      std::string battery_str{};
      for (const auto b : batt_line_idxs) {
        battery_str.push_back(line[b]);
      }
      p1_sum += std::stol(battery_str);
    }

    std::array<size_t, 12> batt_line_idxs{n - 12, n - 11, n - 10, n - 9,
                                          n - 8,  n - 7,  n - 6,  n - 5,
                                          n - 4,  n - 3,  n - 2,  n - 1};
    for (int i{static_cast<int>(n) - 13}; i >= 0; --i) {
      size_t curr_line_idx = static_cast<size_t>(i);
      for (size_t batt_index{0}; batt_index < std::ssize(batt_line_idxs);
           ++batt_index) {
        auto batt_line_idx = batt_line_idxs[batt_index];
        if (line[curr_line_idx] >= line[batt_line_idx] &&
            curr_line_idx < batt_line_idx) {
          if (batt_index && (curr_line_idx) < batt_line_idxs[batt_index - 1])
            continue;
          std::swap(curr_line_idx, batt_line_idxs[batt_index]);
        }
      }
    }

    std::string battery_str{};
    for (const auto b : batt_line_idxs) {
      battery_str.push_back(line[b]);
    }
    p2_sum += std::stol(battery_str);
  }

  auto after = high_resolution_clock::now();
  auto diff = duration_cast<microseconds>(after - before).count();
  std::cout << "Part 1: " << p1_sum << '\n';
  std::cout << "Part 2: " << p2_sum << '\n';
  std::cout << diff << "us\n";
}
