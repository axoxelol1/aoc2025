#include <chrono>
#include <fstream>
#include <iostream>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

template <size_t N>
constexpr long best_battery(const std::string& line) {
  std::array<size_t, N> battery_to_line{};
  for (size_t battery{0}; battery < N; ++battery) {
    char max{'0'};
    size_t max_index;
    size_t start_idx = battery == 0 ? 0 : battery_to_line[battery - 1] + 1;
    for (size_t i{start_idx}; i < line.size() - N + 1 + battery; ++i) {
      if (line[i] == '9') {
        battery_to_line[battery] = i;
        goto done;
      } else if (line[i] > max) {
        max = line[i];
        max_index = i;
      }
    }
    battery_to_line[battery] = max_index;
  done:
  }
  long number = 0;
  for (size_t i = 0; i < N; ++i) {
    number = number * 10 + (line[battery_to_line[i]] - '0');
  }
  return number;
}

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

  long sum_p1{0};
  long sum_p2{0};
  std::string line{};
  line.reserve(100);
  while (std::getline(file, line) && !line.empty()) {
    sum_p1 += best_battery<2>(line);
    sum_p2 += best_battery<12>(line);
  }

  auto after = high_resolution_clock::now();
  std::cout << "Sum: " << sum_p1 << '\n';
  std::cout << "Sum: " << sum_p2 << '\n';
  std::cout << "Time: " << duration_cast<microseconds>(after - before).count()
            << " microseconds\n";
}
