#include <chrono>
#include <fstream>
#include <iostream>

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
  int curr{50};
  int zero_count{0};
  int click_count{0};
  char c;
  int x;
  while (true) {
    file >> c >> x;
    if (!file) {
      break;
    }
    click_count += x / 100;
    x %= 100;

    if (c == 'R') {
      if (curr != 0 && (curr + x) > 100)
        ++click_count;
      curr = (curr + x) % 100;
    } else {
      if (curr != 0 && (curr - x) < 0)
        ++click_count;
      curr = (curr - x);
      if (curr < 0)
        curr = 100 + curr;
    }
    if (curr == 0) {
      ++zero_count;
    }
  }
  auto after = high_resolution_clock::now();

  std::cout << "Part 1: " << zero_count << '\n';
  std::cout << "Part 2: " << zero_count + click_count << '\n';

  auto duration = duration_cast<microseconds>(after - before).count();
  std::cout << "Time: " << duration << "µs\n";
}
