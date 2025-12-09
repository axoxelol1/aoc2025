#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

// constexpr std::string filename{"example.txt"};
// constexpr int n_nodes{20};
// constexpr int connections{10};
constexpr std::string filename{"input.txt"};
constexpr int n_nodes{1000};
constexpr int connections{1000};

static std::array<size_t, n_nodes> parents{};
static std::array<size_t, n_nodes> sizes{};
static size_t max_size{1};

size_t find_set(size_t node) {
  auto root = node;
  while (root != parents[root]) {
    root = parents[root];
  }

  auto current = node;
  while (current != root) {
    auto next = parents[current];
    parents[current] = root;
    current = next;
  }

  return root;
}

void union_sets(size_t a, size_t b) {
  a = find_set(a);
  b = find_set(b);
  if (a != b) {
    if (sizes[a] < sizes[b]) {
      std::swap(a, b);
    }
    parents[b] = a;
    sizes[a] += sizes[b];
    max_size = std::max(max_size, sizes[a]);
    sizes[b] = 0;
  }
}

struct Point {
  long x;
  long y;
  long z;
};

struct Edge {
  size_t from;
  size_t to;
  long dist;
};

bool operator<(const Edge& e1, const Edge& e2) { return e1.dist < e2.dist; }
bool operator>(const Edge& e1, const Edge& e2) { return e1.dist > e2.dist; }

std::istream& operator>>(std::istream& in, Point& point) {
  char comma;
  return in >> point.x >> comma >> point.y >> comma >> point.z;
}

long euc_dist(const Point& a, const Point& b) {
  long dx = a.x - b.x;
  long dy = a.y - b.y;
  long dz = a.z - b.z;

  return dx * dx + dy * dy + dz * dz;
}

int main() {
  std::fstream file{filename, std::ios::in};

  std::array<Point, n_nodes> points;
  for (auto& point : points) {
    file >> point;
  }

  std::priority_queue<Edge, std::vector<Edge>, std::greater<>> edges;
  for (size_t u{0}; u < n_nodes; ++u) {
    for (size_t v{u + 1}; v < n_nodes; ++v) {
      edges.push(Edge{u, v, euc_dist(points[u], points[v])});
    }
  }

  std::iota(parents.begin(), parents.end(), 0);
  std::fill(sizes.begin(), sizes.end(), 1);

  for (int i{0}; i < connections; ++i) {
    auto edge = edges.top();
    union_sets(edge.from, edge.to);
    edges.pop();
  }

  size_t first = 0;
  size_t second = 0;
  size_t third = 0;
  for (auto s : sizes) {
    if (s > first) {
      third = second;
      second = first;
      first = s;
    } else if (s > second) {
      third = second;
      second = s;
    } else if (s > third) {
      third = s;
    }
  }

  std::cout << "Part 1: " << first * second * third << '\n';

  size_t latest_from;
  size_t latest_to;
  while (true) {

    auto edge = edges.top();
    union_sets(edge.from, edge.to);
    latest_from = edge.from;
    latest_to = edge.to;
    if (max_size == n_nodes)
      break;
    edges.pop();
  }

  std::cout << "Part 2: " << points[latest_from].x * points[latest_to].x
            << '\n';
}
