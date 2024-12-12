#include "Utils.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
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

    unsigned long long ans = 0;

    while (std::getline(inFile, line))
    {
        auto s = Split(line, " ");
        std::vector<unsigned long long> vals(s.size() - 1);
        std::transform(s.begin() + 1, s.end(), vals.begin(), [](std::string &num) { return std::stoull(num); });
        auto test = std::stoull(s[0].substr(0, s[0].size() - 1));

        std::queue<std::pair<unsigned long long, int>> toDo;
        toDo.emplace(0, 0);

        while (!toDo.empty())
        {
            auto [curTotal, index] = toDo.front();
            toDo.pop();

            if (index == vals.size())
            {
                if (curTotal == test)
                {
                    ans += test;
                    break;
                }
                continue;
            }

            curTotal += vals[index];
            toDo.emplace(curTotal, index + 1);
            curTotal -= vals[index];
            if (index == 0)
            {
                curTotal = vals[index];
            }
            else
            {
                curTotal *= vals[index];
            }
            toDo.emplace(curTotal, index + 1);
        }
    }

    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    while (std::getline(inFile, line))
    {
        auto s = Split(line, " ");
        std::vector<unsigned long long> vals(s.size() - 1);
        std::transform(s.begin() + 1, s.end(), vals.begin(), [](std::string &num) { return std::stoull(num); });
        auto test = std::stoull(s[0].substr(0, s[0].size() - 1));

        std::queue<std::pair<unsigned long long, int>> toDo;
        toDo.emplace(vals[0], 1);

        while (!toDo.empty())
        {
            auto [curTotal, index] = toDo.front();
            toDo.pop();

            if (index == vals.size())
            {
                if (curTotal == test)
                {
                    ans += test;
                    break;
                }
                continue;
            }

            curTotal += vals[index];
            toDo.emplace(curTotal, index + 1);
            curTotal -= vals[index];
            curTotal *= vals[index];
            toDo.emplace(curTotal, index + 1);
            curTotal /= vals[index];
            auto concat = std::stoull(std::to_string(curTotal) + std::to_string(vals[index]));
            toDo.emplace(concat, index + 1);
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