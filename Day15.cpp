#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "Utils.h"

enum Part { One, Two };
constexpr Part PART = Part::Two;

unsigned int HASH(const std::string& s) {
  unsigned int hash = 0;
  for (const auto& c : s) {
    hash += c;
    hash *= 17;
    hash %= 256;
  }
  return hash;
}

class HASHMAP {
 public:
  HASHMAP() : map(std::vector<std::list<std::pair<std::string, int>>>(256)) {}

  void Insert(const std::string& key, int value) {
    auto k = HASH(key);
    auto& l = map[k];
    for (auto& node : l) {
      if (node.first == key) {
        node.second = value;
        return;
      }
    }
    l.emplace_back(key, value);
  }

  void Delete(const std::string& key) {
    auto k = HASH(key);
    auto& l = map[k];
    for (auto itr = l.begin(); itr != l.end(); itr++) {
      if (itr->first == key) {
        l.erase(itr);
        return;
      }
    }
  }

  unsigned int GetPower() {
    unsigned int power = 0;

    for (int i = 0; i < 256; i++) {
      int j = 0;
      for (auto& node : map[i]) {
        power += (i + 1) * (j + 1) * node.second;
        j++;
      }
    }
    return power;
  }

 private:
  std::vector<std::list<std::pair<std::string, int>>> map;
};

void Part1(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  unsigned long long ans = 0;
  while (std::getline(inFile, line)) {
    auto steps = Split(line, ",");

    for (const auto& step : steps) {
      ans += HASH(step);
    }
  }
  std::cout << ans << std::endl;
}

void Part2(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  auto map = HASHMAP();
  unsigned long long ans = 0;
  while (std::getline(inFile, line)) {
    auto steps = Split(line, ",");

    for (const auto& step : steps) {
      if (step.find('=') != std::string::npos) {
        auto split = Split(step, "=");
        map.Insert(split[0], std::stoi(split[1]));
      } else {
        auto split = Split(step, "-");
        map.Delete(split[0]);
      }
    }
  }

  std::cout << map.GetPower() << std::endl;
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