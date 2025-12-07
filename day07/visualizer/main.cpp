#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int INPUT_WIDTH = 141;

std::fstream& operator<<(std::fstream& fs, uint16_t val) {
  fs.write(reinterpret_cast<const char*>(&val), sizeof(val));
  return fs;
}

std::size_t toUZ(std::integral auto x) { return static_cast<std::size_t>(x); }

template <typename T>
bool contains(const std::vector<T>& vec, const T& target) {
  return std::ranges::find(vec, target) != vec.end();
}

std::vector<std::array<long, INPUT_WIDTH>>
part_2(int initial_x, int max_y,
       std::array<std::vector<int>, INPUT_WIDTH>& splitters) {
  std::array<long, INPUT_WIDTH> x_to_timeline_count{};

  x_to_timeline_count[toUZ(initial_x)] = 1;
  std::vector<std::array<long, INPUT_WIDTH>> frame_data = {};
  int y{0};
  while (y < max_y) {
    std::array<long, INPUT_WIDTH> diffs{};
    for (int x{0}; x < INPUT_WIDTH; ++x) {
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
    frame_data.push_back(x_to_timeline_count);
  }
  return frame_data;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Give input file\n";
    return 0;
  }
  std::fstream file{argv[1], std::ios::in};
  if (!file) {
    std::cout << "File not found\n";
    return 0;
  }
  std::array<std::vector<int>, INPUT_WIDTH> splitters{};
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

  auto frame_data = part_2(initial_x, max_y, splitters);
  // long max_conc{1};

  constexpr int MAX_VAL = 65535;
  for (size_t frame{0}; frame < static_cast<size_t>(max_y); ++frame) {
    std::stringstream filename;
    filename << "images/" << std::setw(3) << std::setfill('0') << frame
             << ".ppm";
    std::fstream image{filename.str(), std::ios::out};
    image << "P6\n" << INPUT_WIDTH << ' ' << max_y << '\n' << MAX_VAL << '\n';
    for (size_t y{0}; y < static_cast<size_t>(max_y); ++y) {
      for (size_t x{0}; x < INPUT_WIDTH; ++x) {
        uint16_t r{0};
        uint16_t g{0};
        uint16_t b{0};
        if (y <= frame) {
          auto conc = frame_data[y][x];
          if (conc) {
            auto val{MAX_VAL -
                     (static_cast<int>(frame) - static_cast<int>(y)) * 10};
            if (val > 0)
              g = static_cast<uint16_t>(val);
          }
        }

        if (contains(splitters[x], static_cast<int>(y))) {
          r = MAX_VAL;
        }
        image << r << g << b;
      }
    }
  }
}
