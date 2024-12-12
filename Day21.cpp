#include "Utils.h"
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

enum Part
{
    One,
    Two
};
constexpr Part PART = Part::Two;

enum class Tile
{
    Rock,
    Plot
};

struct Work
{
    int x, y;
    int stepsRemaining;
};

std::vector<std::vector<unsigned int>> ComputeTileDistances(unsigned int startX, unsigned int startY,
                                                            const std::vector<std::vector<Tile>> &map)
{
    std::vector<std::vector<unsigned int>> distanceMap(map.size(),
                                                       std::vector<unsigned int>(map.back().size(), UINT_MAX));

    std::deque<Work> toDo;
    toDo.emplace_back(startX, startY, 0);
    distanceMap[startY][startX] = 0;
    while (!toDo.empty())
    {
        auto [x, y, steps] = toDo.front();
        toDo.pop_front();

        std::vector<std::pair<int, int>> nextPositions = {{x - 1, y}, {x, y - 1}, {x + 1, y}, {x, y + 1}};
        for (auto &[nx, ny] : nextPositions)
        {
            if (nx < 0 || nx >= map.back().size() || ny < 0 || ny >= map.size() || map[ny][nx] == Tile::Rock)
            {
                continue;
            }
            if (distanceMap[ny][nx] <= steps + 1)
            {
                continue;
            }
            distanceMap[ny][nx] = steps + 1;
            toDo.emplace_back(nx, ny, distanceMap[y][x] + 1);
        }
    }
    return distanceMap;
}

unsigned int ComputeReachablePlots(unsigned int startX, unsigned int startY, const std::vector<std::vector<Tile>> &map,
                                   unsigned int numSteps)
{
    auto distanceMap = ComputeTileDistances(startX, startY, map);

    const auto steps_parity = numSteps % 2;
    unsigned int ans = 0;
    for (int y = 0; y < distanceMap.size(); y++)
    {
        for (int x = 0; x < distanceMap.back().size(); x++)
        {
            if (distanceMap[y][x] <= numSteps && distanceMap[y][x] % 2 == steps_parity)
            {
                ans++;
            }
        }
    }
    return ans;
}

using MemoMap = std::vector<std::vector<std::vector<bool>>>;

void Part1(int argc, char **argv)
{
    constexpr auto STEPS = 64;

    std::ifstream inFile(argv[1]);
    std::string line;

    std::vector<std::vector<Tile>> map;
    int startX{}, startY{};

    while (std::getline(inFile, line))
    {
        map.emplace_back();
        for (char c : line)
        {
            map.back().emplace_back((c == '#') ? Tile::Rock : Tile::Plot);
            if (c == 'S')
            {
                startX = map.back().size() - 1;
                startY = map.size() - 1;
            }
        }
    }

    std::cout << ComputeReachablePlots(startX, startY, map, STEPS) << '\n';
}

void Part2(int argc, char **argv)
{
    constexpr auto STEPS = 130;

    std::ifstream inFile(argv[1]);
    std::string line;

    std::vector<std::vector<Tile>> map;
    int startX{}, startY{};

    while (std::getline(inFile, line))
    {
        map.emplace_back();
        for (char c : line)
        {
            map.back().emplace_back((c == '#') ? Tile::Rock : Tile::Plot);
            if (c == 'S')
            {
                startX = map.back().size() - 1;
                startY = map.size() - 1;
            }
        }
    }

    unsigned long long evenTilePlots = ComputeReachablePlots(startX, startY, map, STEPS);
    unsigned long long oddTilePlots = ComputeReachablePlots(startX, startY, map, STEPS - 1);
    constexpr unsigned long long HALF_TILE_SIZE = 131 / 2;
    constexpr unsigned long long NUM_TILES = (26501365 - HALF_TILE_SIZE) / 131;
    unsigned long long ans = evenTilePlots * (NUM_TILES / 2 + 1) * (NUM_TILES / 2 + 1) +
                             oddTilePlots * (NUM_TILES / 2) * (NUM_TILES / 2) + (NUM_TILES / 2) * oddTilePlots / 2 -
                             (NUM_TILES / 2 + 1) * evenTilePlots / 2;
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