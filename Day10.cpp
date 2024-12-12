#include <array>
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <numeric>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "Utils.h"

enum Part
{
    One,
    Two
};
constexpr Part PART = Part::Two;

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::vector<std::vector<int>> map;
    std::vector<std::pair<int, int>> starts;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<int>());
        for (auto c : line)
        {
            map.back().emplace_back(c - '0');
            if (c == '0')
            {
                starts.emplace_back(map.back().size() - 1, map.size() - 1);
            }
        }
    }

    for (auto [sx, sy] : starts)
    {
        std::deque<std::pair<int, int>> work;
        std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map.back().size(), false));
        work.emplace_front(sx, sy);

        while (!work.empty())
        {
            auto [x, y] = work.front();
            work.pop_front();
            if (map[y][x] == 9)
            {
                if (visited[y][x] == false)
                {
                    ans++;
                    visited[y][x] = true;
                }
                continue;
            }

            for (int d = -1; d < 2; d += 2)
            {
                if (x == 1 && y == 3)
                {
                    int z = x + d;
                    z = map.back().size();
                    z = map[y][x + d];
                    z = map[y][x];
                }
                if (x + d >= 0 && x + d < map.back().size() && map[y][x + d] - map[y][x] == 1)
                {
                    work.emplace_front(x + d, y);
                }
                if (y + d >= 0 && y + d < map.size() && map[y + d][x] - map[y][x] == 1)
                {
                    work.emplace_front(x, y + d);
                }
            }
        }
    }

    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::vector<std::vector<int>> map;
    std::vector<std::pair<int, int>> starts;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<int>());
        for (auto c : line)
        {
            map.back().emplace_back(c - '0');
            if (c == '0')
            {
                starts.emplace_back(map.back().size() - 1, map.size() - 1);
            }
        }
    }

    for (auto [sx, sy] : starts)
    {
        std::deque<std::pair<int, int>> work;
        std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map.back().size(), false));
        work.emplace_front(sx, sy);

        while (!work.empty())
        {
            auto [x, y] = work.front();
            work.pop_front();
            if (map[y][x] == 9)
            {
                ans++;
                continue;
            }

            for (int d = -1; d < 2; d += 2)
            {
                if (x == 1 && y == 3)
                {
                    int z = x + d;
                    z = map.back().size();
                    z = map[y][x + d];
                    z = map[y][x];
                }
                if (x + d >= 0 && x + d < map.back().size() && map[y][x + d] - map[y][x] == 1)
                {
                    work.emplace_front(x + d, y);
                }
                if (y + d >= 0 && y + d < map.size() && map[y + d][x] - map[y][x] == 1)
                {
                    work.emplace_front(x, y + d);
                }
            }
        }
    }

    std::cout << ans << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    if (PART == Part::One)
    {
        Part1(argc, argv);
    }
    else if (PART == Part::Two)
    {
        Part2(argc, argv);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << std::endl;
}