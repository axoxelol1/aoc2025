#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

struct Point {
  long x;
  long y;
};

using Edge = std::pair<Point, Point>;

std::istream& operator>>(std::istream& in, Point& p) {
  char comma;
  return in >> p.x >> comma >> p.y;
}

std::ostream& operator<<(std::ostream& out, const Point& p) {
  return out << '(' << p.x << ',' << p.y << ')';
}

bool operator==(const Point p1, const Point p2) {
  return p1.x == p2.x && p1.y == p2.y;
}

long area(const Point p1, const Point p2) {
  return (std::abs(p1.x - p2.x) + 1) * (std::abs(p1.y - p2.y) + 1);
}

bool intersect(const Edge e1, const Edge e2) {
  bool e1_horiz = (e1.first.y == e1.second.y);
  bool e2_horiz = (e2.first.y == e2.second.y);

  if (e1_horiz == e2_horiz) {
    return false;
  }

  const Edge& h_edge = e1_horiz ? e1 : e2;
  const Edge& v_edge = e1_horiz ? e2 : e1;

  long h_y = h_edge.first.y;
  long h_x_min = std::min(h_edge.first.x, h_edge.second.x);
  long h_x_max = std::max(h_edge.first.x, h_edge.second.x);

  long v_x = v_edge.first.x;
  long v_y_min = std::min(v_edge.first.y, v_edge.second.y);
  long v_y_max = std::max(v_edge.first.y, v_edge.second.y);

  return (h_x_min < v_x && v_x < h_x_max) && (v_y_min < h_y && h_y < v_y_max);
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

  std::vector<Point> red_tiles;

  Point p;
  while (file >> p)
    red_tiles.push_back(p);

  long max_area_p1{0};
  for (auto p1 : red_tiles) {
    for (auto p2 : red_tiles) {
      if (p1 == p2)
        continue;
      max_area_p1 = std::max(max_area_p1, area(p1, p2));
    }
  }

  std::vector<Edge> polygon_edges;
  for (size_t i{0}; i < red_tiles.size(); ++i) {
    Point q1 = red_tiles[i];
    Point q2 = red_tiles[(i + 1) % red_tiles.size()];
    polygon_edges.emplace_back(q1, q2);
  }

  long max_area_p2{0};
  for (auto p1 : red_tiles) {
    for (auto p2 : red_tiles) {
      if (p1 == p2)
        continue;

      Point C_B = {p2.x, p1.y};
      Point C_D = {p1.x, p2.y};

      int cb_count{0};
      int cd_count{0};
      bool cb_on_edge{false};
      bool cd_on_edge{false};
      for (auto poly_edge : polygon_edges) {
        auto min_x = std::min(poly_edge.first.x, poly_edge.second.x);
        auto max_x = std::max(poly_edge.first.x, poly_edge.second.x);
        auto min_y = std::min(poly_edge.first.y, poly_edge.second.y);
        auto max_y = std::max(poly_edge.first.y, poly_edge.second.y);
        bool vertical = (poly_edge.first.x == poly_edge.second.x);
        if (!vertical) {
          auto y = poly_edge.first.y;
          if (C_B.y == y && min_x <= C_B.x && C_B.x <= max_x)
            cb_on_edge = true;
          if (C_D.y == y && min_x <= C_D.x && C_D.x <= max_x)
            cd_on_edge = true;
          continue;
        }
        auto x = poly_edge.first.x;
        if (C_B.x == x && min_y <= C_B.y && C_B.y <= max_y)
          cb_on_edge = true;
        if (C_D.x == x && min_y <= C_D.y && C_D.y <= max_y) {
          cd_on_edge = true;
        }
        if (C_B.x < x && min_y < C_B.y && C_B.y <= max_y) {
          cb_count += 1;
        }
        if (C_D.x < x && min_y < C_D.y && C_D.y <= max_y) {
          cd_count += 1;
        }
      }
      std::vector<Edge> rectangle_edges = {
          {p1, C_B}, {C_B, p2}, {p2, C_D}, {C_D, p1}};
      if ((!cb_on_edge && cb_count % 2 == 0) ||
          (!cd_on_edge && cd_count % 2 == 0)) {
        goto invalid; // dirty :^)
      }

      for (auto rect_edge : rectangle_edges) {
        for (auto poly_edge : polygon_edges) {
          if (intersect(rect_edge, poly_edge)) {
            goto invalid;
          }
        }
      }
      max_area_p2 = std::max(max_area_p2, area(p1, p2));
    invalid:
    }
  }
  std::cout << "Part 1: " << max_area_p1 << '\n';
  std::cout << "Part 2: " << max_area_p2 << '\n';
}
