#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Utils.h"

enum Part { One, Two };
constexpr Part PART = Part::Two;

void Part1(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  unsigned long long ans = 0;
  std::vector<std::string> pattern;
  std::vector<int> north;
  std::vector<int> load;
  while (std::getline(inFile, line)) {
    if (north.size() == 0) {
      north = std::vector<int>(line.size(), 0);
    }
    load.emplace_back(0);
    for (int i = 0; i < line.size(); ++i) {
      const auto& c = line[i];
      switch (c) {
        case 'O':
          load[north[i]]++;
          north[i]++;
          break;
        case '#':
          north[i] = load.size();
          break;
        default:
          break;
      }
    }
  }
  for (int i = 0; i < load.size(); ++i) {
    ans += load[i] * (load.size() - i);
  }
  std::cout << ans << std::endl;
}

void MoveUp(std::vector<std::string>& pattern) {
  std::vector<int> nextPos = std::vector<int>(pattern[0].size(), 0);
  for (int row = 0; row < pattern[0].size(); row++) {
    for (int col = 0; col < pattern.size(); ++col) {
      const auto& c = pattern[row][col];
      switch (c) {
        case 'O':
          pattern[row][col] = '.';
          pattern[nextPos[col]][col] = 'O';
          nextPos[col]++;
          break;
        case '#':
          nextPos[col] = row + 1;
          break;
        default:
          break;
      }
    }
  }
}

void MoveDown(std::vector<std::string>& pattern) {
  std::vector<int> nextPos =
      std::vector<int>(pattern[0].size(), pattern[0].size() - 1);
  for (int row = pattern[0].size() - 1; row >= 0; --row) {
    for (int col = 0; col < pattern.size(); ++col) {
      const auto& c = pattern[row][col];
      switch (c) {
        case 'O':
          pattern[row][col] = '.';
          pattern[nextPos[col]][col] = 'O';
          nextPos[col]--;
          break;
        case '#':
          nextPos[col] = row - 1;
          break;
        default:
          break;
      }
    }
  }
}

void MoveLeft(std::vector<std::string>& pattern) {
  std::vector<int> nextPos = std::vector<int>(pattern.size(), 0);
  for (int col = 0; col < pattern.size(); ++col) {
    for (int row = 0; row < pattern.size(); ++row) {
      const auto& c = pattern[row][col];
      switch (c) {
        case 'O':
          pattern[row][col] = '.';
          pattern[row][nextPos[row]] = 'O';
          nextPos[row]++;
          break;
        case '#':
          nextPos[row] = col + 1;
          break;
        default:
          break;
      }
    }
  }
}

void MoveRight(std::vector<std::string>& pattern) {
  std::vector<int> nextPos =
      std::vector<int>(pattern.size(), pattern.size() - 1);
  for (int col = pattern.size() - 1; col >= 0; --col) {
    for (int row = 0; row < pattern.size(); ++row) {
      const auto& c = pattern[row][col];
      switch (c) {
        case 'O':
          pattern[row][col] = '.';
          pattern[row][nextPos[row]] = 'O';
          nextPos[row]--;
          break;
        case '#':
          nextPos[row] = col - 1;
          break;
        default:
          break;
      }
    }
  }
}

std::bitset<20000> GetBitSet(const std::vector<std::string>& pattern) {
  std::bitset<20000> bitset{};
  int i = 0;
  for (const auto& line : pattern) {
    for (const auto& c : line) {
      switch (c) {
        case 'O':
          bitset.set(i, 1);
          bitset.set(i + 1, 1);
          break;
        case '#':
          bitset.set(i, 1);
          bitset.set(i + 1, 0);
          break;
        case '.':
          bitset.set(i, 0);
          bitset.set(i + 1, 0);
          break;
      }
      i += 2;
    }
  }
  return bitset;
}

void Part2(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  unsigned long long ans = 0;
  std::vector<std::string> pattern;
  std::vector<int> north;
  std::vector<int> load;
  while (std::getline(inFile, line)) {
    pattern.emplace_back(line);
  }

  std::unordered_map<std::bitset<20000>, std::pair<unsigned int, unsigned int>>
      memo;

  constexpr unsigned int ITERATIONS = 1000000000;
  for (unsigned int i = 0; i < ITERATIONS; i++) {
    auto cachedMap = GetBitSet(pattern);

    if (memo.find(cachedMap) == memo.end()) {
      memo.emplace(std::move(cachedMap), std::make_pair(i, 0));
    } else if (memo[cachedMap].second == 0) {
      memo[cachedMap].second = i;
    } else {
      unsigned int mult =
          (ITERATIONS - i) / (memo[cachedMap].second - memo[cachedMap].first);
      if (mult != 0) {
        i += mult * (memo[cachedMap].second - memo[cachedMap].first) - 1;
        continue;
      }
    }

    MoveUp(pattern);
    MoveLeft(pattern);
    MoveDown(pattern);
    MoveRight(pattern);
  }

  int i = pattern.size();
  for (const auto& row : pattern) {
    for (const auto& c : row) {
      if (c == 'O') {
        ans += i;
      }
    }
    i--;
  }
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