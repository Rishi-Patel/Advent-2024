#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
    int ans = 0;
    std::vector<int> left, right;

    while (std::getline(inFile, line))
    {
        auto split = Split(line, "   ");
        left.emplace_back(std::stoi(split[0]));
        right.emplace_back(std::stoi(split[1]));
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    for (int i = 0; i < left.size(); i++)
    {
        ans += std::abs(left[i] - right[i]);
    }
    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;
    int ans = 0;
    std::unordered_map<int, int> right;
    std::vector<int> left;

    while (std::getline(inFile, line))
    {
        auto split = Split(line, "   ");
        left.emplace_back(std::stoi(split[0]));
        right[std::stoi(split[1])]++;
    }

    for (auto val : left)
    {
        ans += val * right[val];
    }
    std::cout << ans << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
    }

    if (PART == Part::One)
    {
        Part1(argc, argv);
    }
    else if (PART == Part::Two)
    {
        Part2(argc, argv);
    }
}