#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

constexpr int YOU = 'y' << 16 | 'o' << 8 | 'u';
constexpr int OUT = 'o' << 16 | 'u' << 8 | 't';
constexpr int SVR = 's' << 16 | 'v' << 8 | 'r';
constexpr int DAC = 'd' << 16 | 'a' << 8 | 'c';
constexpr int FFT = 'f' << 16 | 'f' << 8 | 't';

constexpr int DAC_BIT = 1 << 0;
constexpr int FFT_BIT = 1 << 1;
using Memo = std::unordered_map<int, long long>;
long long find_path_count(int node, int visited_mask,
                          const std::unordered_map<int, std::vector<int>>& adj,
                          Memo& memo, bool p2) {
  int state = (node << 2) | visited_mask;
  if (memo.contains(state)) {
    return memo.at(state);
  }

  if (node == OUT) {
    if (!p2)
      return 1;
    if (visited_mask == (DAC_BIT | FFT_BIT))
      return 1;
    else
      return 0;
  }

  long long path_count = 0;
  auto neighbors = adj.find(node);
  if (neighbors != adj.end()) {
    for (const int neighbor : adj.at(node)) {

      int new_mask = visited_mask;
      if (neighbor == DAC) {
        new_mask |= DAC_BIT;
      } else if (neighbor == FFT) {
        new_mask |= FFT_BIT;
      }

      path_count += find_path_count(neighbor, new_mask, adj, memo, p2);
    }
  }

  memo[state] = path_count;
  return path_count;
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

  std::string line{};
  std::unordered_map<int, std::vector<int>> adj;
  while (std::getline(file, line) && !line.empty()) {
    int first_node{0};
    for (auto word : std::views::split(line, ' ')) {
      int node =
          *word.begin() << 16 | *(word.begin() + 1) << 8 | *(word.begin() + 2);
      if (!first_node) {
        first_node = node;
        adj[first_node] = {};
      } else {
        adj[first_node].push_back(node);
      }
    }
  }

  Memo memo;
  auto p1_count = find_path_count(YOU, 0, adj, memo, false);
  std::cout << "Part 1: " << p1_count << "\n";
  memo.clear();
  auto p2_count = find_path_count(SVR, 0, adj, memo, true);
  std::cout << "Part 2: " << p2_count << '\n';
}
