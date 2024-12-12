#include <chrono>
#include <fstream>
#include <iostream>
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

std::pair<int, int> NextCoord(const std::pair<int, int> &coord, const int steps, const char direction)
{
    switch (direction)
    {
    case 'U':
        return {coord.first, coord.second + steps};
    case 'D':
        return {coord.first, coord.second - steps};
    case 'L':
        return {coord.first - steps, coord.second};
    case 'R':
        return {coord.first + steps, coord.second};
    }
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    std::vector<std::pair<int, int>> path{{0, 0}};
    int perimeter{};

    int area = 0;
    while (std::getline(inFile, line))
    {
        char direction = line[0];
        int numSteps = std::stoi(line.substr(line.find(" ") + 1, line.find(" ", line.find(" ") + 1) - 2));
        perimeter += numSteps;

        path.emplace_back(NextCoord(path.back(), numSteps, direction));
    }
    for (int i = 0; i < path.size() - 1; i++)
    {
        area += path[i].first * path[i + 1].second - path[i].second * path[i + 1].first;
    }
    area = (area < 0) ? -area : area;
    area = (area >> 1) + (perimeter >> 1) + 1;
    std::cout << area << std::endl;
}

int ConvertHexToInt(std::string hex)
{
    return std::stol(hex, nullptr, 16);
}

char GetDirection(int digit)
{
    switch (digit)
    {
    case 0:
        return 'R';
    case 1:
        return 'D';
    case 2:
        return 'L';
    case 3:
        return 'U';
    }
}
void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    std::vector<std::pair<long long, long long>> path{{0, 0}};
    long long perimeter{};

    long long area = 0;
    while (std::getline(inFile, line))
    {
        char direction = GetDirection(line[line.find(')') - 1] - '0');
        int numSteps = ConvertHexToInt(line.substr(line.find('#') + 1, 5));
        perimeter += numSteps;

        path.emplace_back(NextCoord(path.back(), numSteps, direction));
    }
    for (int i = 0; i < path.size() - 1; i++)
    {
        area += path[i].first * path[i + 1].second - path[i].second * path[i + 1].first;
    }
    area = (area < 0) ? -area : area;
    area = (area >> 1) + (perimeter >> 1) + 1;
    std::cout << area << std::endl;
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