#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

bool is_invalid_p1(long long id) {
  auto digits = std::to_string(id);

  if (digits.size() % 2 != 0) {
    return false;
  }
  for (size_t i{0}; i < digits.size() / 2; ++i) {
    if (digits[i] != digits[digits.size() / 2 + i]) {
      return false;
    }
  }
  return true;
}

bool is_invalid_p2(long long id) {
  auto digits = std::to_string(id);

  if (digits.size() == 1) {
    return false;
  }
  if (digits.size() == 2) {
    if (digits[0] == digits[1]) {
      return digits[0] == digits[1];
    }
  }

  for (size_t start{1}; start < digits.size() / 2 + 1; ++start) {
    if (digits.size() % start != 0)
      continue;
    bool repeated{true};
    for (size_t i{0}; i < digits.size(); ++i) {
      if (digits[i] != digits[i % start]) {
        repeated = false;
        break;
      }
    }
    if (repeated) {
      return true;
    }
  }
  return false;
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
  long long sum_p1{0};
  long long sum_p2{0};
  char ignore;
  long long lower{0};
  long long upper{0};
  while (file) {
    file >> lower >> ignore >> upper >> ignore;
    auto invalid_ids_p1 =
        std::views::iota(lower, upper + 1) | std::views::filter(is_invalid_p1);
    sum_p1 += std::ranges::fold_left(invalid_ids_p1, 0LL, std::plus<>());
    auto invalid_ids_p2 =
        std::views::iota(lower, upper + 1) | std::views::filter(is_invalid_p2);
    sum_p2 += std::ranges::fold_left(invalid_ids_p2, 0LL, std::plus<>());
  }

  auto after = high_resolution_clock::now();
  auto diff = duration_cast<milliseconds>(after - before).count();
  std::cout << "Part 1: " << sum_p1 << '\n';
  std::cout << "Part 2: " << sum_p2 << '\n';
  std::cout << diff << "ms\n";
}
