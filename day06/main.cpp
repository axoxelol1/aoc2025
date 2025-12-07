#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// No cleanup done, read at own risk
// Would be interesting to use the empty cols parsing
// for part 1 as well and avoid expensive conversion
// to 2d vector, might come back to it at some point.

void part_1(std::fstream& file) {
  std::vector<std::vector<long>> rows;
  std::vector<char> operators;

  std::string line;
  while (true) {
    std::vector<long> row;
    row.reserve(1000);

    std::getline(file, line);

    if (line[0] == '*' || line[0] == '+')
      break;

    std::stringstream line_stream{line};
    while (true) {
      long number;
      line_stream >> std::ws >> number;
      if (!line_stream)
        break;
      row.push_back(number);
    }
    rows.push_back(row);
  }

  std::stringstream line_stream{line};
  while (true) {
    char op;
    line_stream >> std::ws >> op;
    if (!line_stream)
      break;
    operators.push_back(op);
  }

  assert(operators.size() == rows[0].size() &&
         rows[0].size() == rows[rows.size() - 1].size());

  long part1_result = 0;
  for (size_t col = 0; col < rows[0].size(); ++col) {
    long result = rows[0][col];
    bool addition = operators[col] == '+';
    for (size_t row = 1; row < rows.size(); ++row) {
      if (addition) {
        result += rows[row][col];
      } else {
        result *= rows[row][col];
      }
    }
    part1_result += result;
  }
  std::cout << "Part 1: " << part1_result << "\n";
}

void part_2(std::fstream& file) {
  std::vector<std::string> rows;
  std::vector<char> operators;

  std::string line;
  while (true) {
    std::vector<std::string> row;
    row.reserve(1000);

    std::getline(file, line);
    if (line[0] == '*' || line[0] == '+')
      break;
    rows.push_back(line + ' ');
  }

  std::stringstream line_stream{line};
  while (true) {
    char op;
    line_stream >> std::ws >> op;
    if (!line_stream)
      break;
    operators.push_back(op);
  }

  // find cols only spaces
  std::vector<size_t> empty_cols;
  for (size_t col = 0; col < rows[0].size(); ++col) {
    bool all_spaces = true;
    for (size_t row = 0; row < rows.size(); ++row) {
      if (rows[row][col] != ' ') {
        all_spaces = false;
        break;
      }
    }
    if (all_spaces) {
      empty_cols.push_back(col);
    }
  }
  long p2_grand_total{0};
  for (size_t i{0}; i < empty_cols.size(); ++i) {
    size_t first_col = (i == 0) ? 0 : empty_cols[i - 1] + 1;
    std::vector<long> numbers;
    for (size_t col{first_col}; col < empty_cols[i]; ++col) {
      std::string number;
      for (size_t row = 0; row < rows.size(); ++row) {
        if (rows[row][col] != ' ')
          number += rows[row][col];
      }
      numbers.push_back(std::stol(number));
    }
    long result = numbers[0];
    for (size_t n = 1; n < numbers.size(); ++n) {
      if (operators[i] == '+') {
        result += numbers[n];
      } else {
        result *= numbers[n];
      }
    }
    p2_grand_total += result;
  }
  std::cout << "Part 2: " << p2_grand_total << "\n";
}

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
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

  auto before = high_resolution_clock::now();
  part_1(file);
  file = std::fstream{argv[1], std::ios::in};
  auto after_p1 = high_resolution_clock::now();
  part_2(file);
  auto after_p2 = high_resolution_clock::now();
  std::cout << "Time (p1):  "
            << duration_cast<microseconds>(after_p1 - before).count()
            << " us\n";
  std::cout << "Time (p2):  "
            << duration_cast<microseconds>(after_p2 - after_p1).count()
            << " us\n";
  std::cout << "Total time: "
            << duration_cast<microseconds>(after_p2 - before).count()
            << " us\n";
}
