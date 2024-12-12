#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "Utils.h"

enum Part { One, Two };
constexpr Part PART = Part::Two;

enum class Direction { Unknown = 0, Up, Down, Left, Right };

struct Laser {
  int x, y;
  Direction d;

  void Move() {
    switch (d) {
      case Direction::Up:
        y--;
        break;
      case Direction::Down:
        y++;
        break;
      case Direction::Left:
        x--;
        break;
      case Direction::Right:
        x++;
        break;
    }
  }
};

void Part1(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  std::vector<std::string> map;
  std::vector<std::vector<bool>> energized;
  std::vector<std::vector<bool>> splitVisited;

  while (std::getline(inFile, line)) {
    map.emplace_back(line);
    energized.emplace_back(std::vector<bool>(line.size(), false));
    splitVisited.emplace_back(std::vector<bool>(line.size(), false));
  }

  unsigned long long ans = 0;
  std::deque<Laser> toDo;
  toDo.emplace_back(0, 0, Direction::Right);

  while (!toDo.empty()) {
    // for (auto& row : energized) {
    //   for (auto v : row) {
    //     std::cout << ((v) ? '#' : '.');
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << std::endl;

    auto laser = std::move(toDo.front());
    toDo.pop_front();

    if (laser.x < 0 || laser.x >= map[0].size()) {
      continue;
    }
    if (laser.y < 0 || laser.y >= map.size()) {
      continue;
    }

    // Splits are the only way a laser may cycle through a path
    // so avoiding splits that have already been seen saves us from
    // checking cycles
    if (splitVisited[laser.y][laser.x]) {
      continue;
    }

    energized[laser.y][laser.x] = true;
    char c = map[laser.y][laser.x];
    switch (c) {
      case '.':
        laser.Move();
        toDo.emplace_back(laser);
        continue;
      case '/':
        switch (laser.d) {
          case Direction::Up:
            laser.d = Direction::Right;
            break;
          case Direction::Down:
            laser.d = Direction::Left;
            break;
          case Direction::Left:
            laser.d = Direction::Down;
            break;
          case Direction::Right:
            laser.d = Direction::Up;
            break;
        }
        laser.Move();
        toDo.emplace_back(laser);
        break;
      case '\\':
        switch (laser.d) {
          case Direction::Up:
            laser.d = Direction::Left;
            break;
          case Direction::Down:
            laser.d = Direction::Right;
            break;
          case Direction::Left:
            laser.d = Direction::Up;
            break;
          case Direction::Right:
            laser.d = Direction::Down;
            break;
        }
        laser.Move();
        toDo.emplace_back(laser);
        break;
      case '-':
        switch (laser.d) {
          case Direction::Up:
          case Direction::Down:
            toDo.emplace_back(laser.x + 1, laser.y, Direction::Right);
            toDo.emplace_back(laser.x - 1, laser.y, Direction::Left);
            splitVisited[laser.y][laser.x] = true;
            break;
          case Direction::Left:
          case Direction::Right:
            laser.Move();
            toDo.emplace_back(laser);
            break;
        }
        break;
      case '|':
        switch (laser.d) {
          case Direction::Left:
          case Direction::Right:
            toDo.emplace_back(laser.x, laser.y + 1, Direction::Down);
            toDo.emplace_back(laser.x, laser.y - 1, Direction::Up);
            splitVisited[laser.y][laser.x] = true;
            break;
          case Direction::Up:
          case Direction::Down:
            laser.Move();
            toDo.emplace_back(laser);
            break;
        }
        break;
    }
  }

  for (const auto& line : energized) {
    for (auto b : line) {
      if (b) {
        ans++;
      }
    }
  }
  std::cout << ans << std::endl;
}

void Part2(int argc, char** argv) {
  std::ifstream inFile(argv[1]);
  std::string line;

  std::vector<std::string> map;
  std::vector<std::vector<bool>> energized;
  std::vector<std::vector<bool>> splitVisited;

  while (std::getline(inFile, line)) {
    map.emplace_back(line);
    energized.emplace_back(std::vector<bool>(line.size(), false));
    splitVisited.emplace_back(std::vector<bool>(line.size(), false));
  }

  unsigned long long ans = 0;
  std::deque<Laser> toDo;

  std::vector<Laser> starts;
  for (int i = 0; i < map.size(); ++i) {
    starts.emplace_back(0, i, Direction::Right);
    starts.emplace_back(i, 0, Direction::Down);
    starts.emplace_back(map.size() - 1, map.size() - 1 - i, Direction::Left);
    starts.emplace_back(map.size() - 1 - i, map.size() - 1, Direction::Up);
  }

  for (const auto& start : starts) {
    for (int i = 0; i < map.size(); i++) {
      for (int j = 0; j < map.size(); j++) {
        energized[i][j] = false;
        splitVisited[i][j] = false;
      }
    }

    toDo.emplace_back(start);

    while (!toDo.empty()) {
      // for (auto& row : energized) {
      //   for (auto v : row) {
      //     std::cout << ((v) ? '#' : '.');
      //   }
      //   std::cout << std::endl;
      // }
      // std::cout << std::endl;

      auto laser = std::move(toDo.front());
      toDo.pop_front();

      if (laser.x < 0 || laser.x >= map[0].size()) {
        continue;
      }
      if (laser.y < 0 || laser.y >= map.size()) {
        continue;
      }

      // Splits are the only way a laser may cycle through a path
      // so avoiding splits that have already been seen saves us from
      // checking cycles
      if (splitVisited[laser.y][laser.x]) {
        continue;
      }

      energized[laser.y][laser.x] = true;
      char c = map[laser.y][laser.x];
      switch (c) {
        case '.':
          laser.Move();
          toDo.emplace_back(laser);
          continue;
        case '/':
          switch (laser.d) {
            case Direction::Up:
              laser.d = Direction::Right;
              break;
            case Direction::Down:
              laser.d = Direction::Left;
              break;
            case Direction::Left:
              laser.d = Direction::Down;
              break;
            case Direction::Right:
              laser.d = Direction::Up;
              break;
          }
          laser.Move();
          toDo.emplace_back(laser);
          break;
        case '\\':
          switch (laser.d) {
            case Direction::Up:
              laser.d = Direction::Left;
              break;
            case Direction::Down:
              laser.d = Direction::Right;
              break;
            case Direction::Left:
              laser.d = Direction::Up;
              break;
            case Direction::Right:
              laser.d = Direction::Down;
              break;
          }
          laser.Move();
          toDo.emplace_back(laser);
          break;
        case '-':
          switch (laser.d) {
            case Direction::Up:
            case Direction::Down:
              toDo.emplace_back(laser.x + 1, laser.y, Direction::Right);
              toDo.emplace_back(laser.x - 1, laser.y, Direction::Left);
              splitVisited[laser.y][laser.x] = true;
              break;
            case Direction::Left:
            case Direction::Right:
              laser.Move();
              toDo.emplace_back(laser);
              break;
          }
          break;
        case '|':
          switch (laser.d) {
            case Direction::Left:
            case Direction::Right:
              toDo.emplace_back(laser.x, laser.y + 1, Direction::Down);
              toDo.emplace_back(laser.x, laser.y - 1, Direction::Up);
              splitVisited[laser.y][laser.x] = true;
              break;
            case Direction::Up:
            case Direction::Down:
              laser.Move();
              toDo.emplace_back(laser);
              break;
          }
          break;
      }
    }

    unsigned int count = 0;
    for (const auto& line : energized) {
      for (auto b : line) {
        if (b) {
          count++;
        }
      }
    }
    ans = (count > ans) ? count : ans;
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