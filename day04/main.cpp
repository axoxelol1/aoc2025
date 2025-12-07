#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

// constexpr int GRID_SIZE = 10;
// constexpr std::string INPUT = "example.txt";
constexpr int GRID_SIZE = 136;
constexpr std::string INPUT = "input.txt";

int neighbour_count(const std::array<bool, GRID_SIZE * GRID_SIZE>& grid,
                    int row, int col) {
  int count{0};
  for (int dr = -1; dr <= 1; ++dr) {
    for (int dc = -1; dc <= 1; ++dc) {
      if (dr == 0 && dc == 0)
        continue;
      if ((row == 0 && dr == -1) || (row == GRID_SIZE - 1 && dr == 1) ||
          (col == 0 && dc == -1) || (col == GRID_SIZE - 1 && dc == 1))
        continue;
      if (grid[static_cast<size_t>((row + dr) * GRID_SIZE + (col + dc))]) {
        ++count;
      }
    }
  }
  return count;
}

int main() {
  auto before = high_resolution_clock::now();
  std::fstream file{INPUT, std::ios::in};
  if (!file) {
    std::cout << "File not found\n";
    return 0;
  }
  std::array<bool, GRID_SIZE * GRID_SIZE> grid;
  char c;
  size_t i{0};
  while (file >> c) {
    if (c == '@' || c == '.') {
      grid[i++] = c == '@';
    }
  }

  std::vector<size_t> accessible_indexes{};
  int removed_total{0};
  int removed_p1{0};
  while (true) {
    for (int row = 0; row < GRID_SIZE; ++row) {
      for (int col = 0; col < GRID_SIZE; ++col) {
        if (!grid[static_cast<size_t>(row * GRID_SIZE + col)])
          continue;
        if (neighbour_count(grid, row, col) < 4) {
          accessible_indexes.push_back(
              static_cast<size_t>(row * GRID_SIZE + col));
        }
      }
    }
    if (accessible_indexes.empty()) {
      break;
    }
    for (const auto index : accessible_indexes) {
      grid[index] = false;
      ++removed_total;
    }
    if (removed_p1 == 0)
      removed_p1 = removed_total;
    accessible_indexes.clear();
  }

  auto after = high_resolution_clock::now();
  std::cout << "Part 1: " << removed_p1 << '\n';
  std::cout << "Part 2: " << removed_total << '\n';
  std::cout << "Time: " << duration_cast<microseconds>(after - before).count()
            << "us\n";
}
