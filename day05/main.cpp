#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

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
  std::vector<std::pair<long, long>> ranges;
  std::vector<long> ids;
  ids.reserve(1000);
  ranges.reserve(192);
  while (!(file.peek() == '\n')) {
    long long x;
    long long y;
    char dash;
    file >> x >> dash >> y;
    file.ignore(1); // Ignore newline
    ranges.emplace_back(x, y);
  }
  while (true) {
    long long x;
    file >> x;
    if (!file)
      break;
    ids.emplace_back(x);
  }

  std::ranges::sort(ranges, {}, [](const auto& p) { return p.first; });
  std::vector<std::pair<long, long>> merged_ranges{};
  merged_ranges.reserve(192);

  for (const auto& range : ranges) {
    if (merged_ranges.empty() || merged_ranges.back().second < range.first) {
      merged_ranges.push_back(range);
    } else {
      merged_ranges.back().second =
          std::max(merged_ranges.back().second, range.second);
    }
  }

  // Part 1
  int p1_count{0};
  for (const auto id : ids) {
    for (const auto& range : merged_ranges) {
      if (id >= range.first && id <= range.second) {
        p1_count++;
        break;
      }
    }
  }

  // Part 2
  long p2_count{0};
  for (const auto& range : merged_ranges) {
    p2_count += (range.second - range.first + 1);
  }

  auto after = high_resolution_clock::now();
  std::cout << "Part 1: " << p1_count << "\n";
  std::cout << "Part 2: " << (p2_count) << "\n";
  std::cout << "Time: " << duration_cast<microseconds>(after - before).count()
            << "us\n";
}
