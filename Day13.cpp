#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Utils.h"

enum Part { One, Two };
constexpr Part PART = Part::Two;

unsigned long long FindReflections(const std::vector<std::string>& pattern) {
  unsigned long long ans = 0;
  int numCols = pattern.begin()->size();
  int numRows = pattern.size();
  for (int v = 0; v < numCols - 1; v++) {
    int len = std::min(v + 1, numCols - v - 1);
    bool isReflection = true;
    for (int row = 0; row < numRows; row++) {
      int offset = 0;
      while (offset != len) {
        if (pattern[row][v - offset] != pattern[row][v + offset + 1]) {
          isReflection = false;
          goto checkedVReflection;
        }
        offset++;
      }
    }

  checkedVReflection:
    if (isReflection) {
      ans += v + 1;
    }
  }

  for (int h = 0; h < numRows - 1; h++) {
    int len = std::min(h + 1, numRows - h - 1);
    bool isReflection = true;
    for (int col = 0; col < numCols; col++) {
      int offset = 0;
      while (offset != len) {
        if (pattern[h - offset][col] != pattern[h + offset + 1][col]) {
          isReflection = false;
          goto checkedHReflection;
        }
        offset++;
      }
    }

  checkedHReflection:
    if (isReflection) {
      ans += (h + 1) * 100;
    }
  }
  return ans;
}

void Part1(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  unsigned long long ans = 0;
  std::vector<std::string> pattern;
  while (std::getline(inFile, line)) {
    if (line == "") {
      ans += FindReflections(pattern);
      pattern.clear();
      continue;
    }
    pattern.emplace_back(line);
  }
  ans += FindReflections(pattern);

  std::cout << ans << std::endl;
}

struct Point {
  int x;
  int y;
  bool operator==(const Point& p) const {
    return std::tie(p.x, p.y) == std::tie(x, y);
  }
};

template <>
struct std::hash<Point> {
  std::size_t operator()(const Point& p) const {
    using std::hash;
    using std::size_t;
    auto h = hash<int>()(p.x);
    h ^= hash<int>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

unsigned long long FindReflections2(const std::vector<std::string>& pattern) {
  unsigned long long ans = 0;
  int numCols = pattern.begin()->size();
  int numRows = pattern.size();
  std::unordered_map<Point, int> count;
  for (int v = 0; v < numCols - 1; v++) {
    int numWrong = 0;
    int len = std::min(v + 1, numCols - v - 1);
    bool isReflection = true;
    for (int row = 0; row < numRows; row++) {
      int offset = 0;
      while (offset != len) {
        if (pattern[row][v - offset] != pattern[row][v + offset + 1]) {
          isReflection = false;
          numWrong++;
        }
        offset++;
      }
    }
    // std::cout << "V: " << v << " NumWrong: " << numWrong << std::endl;

  checkedVReflection:
    if (numWrong == 1) {
      ans += v + 1;
    }
  }

  for (int h = 0; h < numRows - 1; h++) {
    int len = std::min(h + 1, numRows - h - 1);
    bool isReflection = true;
    int numWrong = 0;
    for (int col = 0; col < numCols; col++) {
      int offset = 0;
      while (offset != len) {
        if (pattern[h - offset][col] != pattern[h + offset + 1][col]) {
          isReflection = false;
          numWrong++;
        }
        offset++;
      }
    }

    // std::cout << "H: " << h << " NumWrong: " << numWrong << std::endl;
  checkedHReflection:
    if (numWrong == 1) {
      ans += (h + 1) * 100;
    }
  }
  return ans;
}

void Part2(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  unsigned long long ans = 0;
  std::vector<std::string> pattern;
  while (std::getline(inFile, line)) {
    if (line == "") {
      ans += FindReflections2(pattern);
      pattern.clear();
      continue;
    }
    pattern.emplace_back(line);
  }
  ans += FindReflections2(pattern);

  std::cout << ans << std::endl;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    return 1;
  }

  auto start = std::chrono::high_resolution_clock::now();
  if (PART == Part::One) {
    Part1(argc, argv);
  } else if (PART == Part::Two) {
    Part2(argc, argv);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << duration.count() << std::endl;
}