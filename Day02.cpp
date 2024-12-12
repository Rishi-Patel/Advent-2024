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

bool isSafe(std::vector<int> &vals)
{
    bool isIncreasing = (vals[1] > vals[0]);
    for (int i = 1; i < vals.size(); i++)
    {
        int diff = vals[i] - vals[i - 1];
        if (std::abs(diff) < 1 || std::abs(diff) > 3)
        {
            return false;
        }
        if (isIncreasing && vals[i] < vals[i - 1] || !isIncreasing && vals[i] > vals[i - 1])
        {
            return false;
        }
    }

    return true;
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;
    int ans = 0;

    while (std::getline(inFile, line))
    {
        std::vector<int> vals;
        auto split = Split(line, " ");
        for (auto str : split)
        {
            vals.emplace_back(std::stoi(str));
        }

        if (isSafe(vals))
        {
            ans++;
        }
    }

    std::cout << ans << std::endl;
}

bool isSafe2(std::vector<int> &vals, bool canRemove)
{
    if (vals.size() < 2)
    {
        return true;
    }

    bool isIncreasing = (vals[1] > vals[0]);
    for (int i = 1; i < vals.size(); i++)
    {
        int diff = vals[i] - vals[i - 1];
        bool isUnSafe = (std::abs(diff) < 1) || (std::abs(diff) > 3) || (isIncreasing && vals[i] < vals[i - 1]) ||
                        (!isIncreasing && vals[i] > vals[i - 1]);
        if (isUnSafe)
        {
            if (canRemove)
            {
                auto v2 = vals;
                v2.erase(v2.begin() + i);
                auto v3 = vals;
                v3.erase(v3.begin() + i - 1);

                auto b1 = isSafe2(v2, false);
                auto b2 = isSafe2(v3, false);
                auto b3 = false;
                if (i == 2)
                {
                    auto v4 = vals;
                    v4.erase(v4.begin());
                    b3 = isSafe2(v4, false);
                }
                if (!b1 && !b2)
                {
                    for (int j = 0; j < vals.size(); j++)
                    {
                        std::cout << vals[j] << " ";
                    }
                    std::cout << std::endl;
                    for (int j = 1; j < vals.size(); j++)
                    {
                        std::cout << vals[j] - vals[j - 1] << " ";
                    }
                    std::cout << std::endl << std::endl;
                }
                return b2 || b1 || b3;
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;
    int ans = 0;

    while (std::getline(inFile, line))
    {
        std::vector<int> vals;
        auto split = Split(line, " ");
        for (auto str : split)
        {
            vals.emplace_back(std::stoi(str));
        }

        if (isSafe2(vals, true))
        {
            ans++;
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

    if (PART == Part::One)
    {
        Part1(argc, argv);
    }
    else if (PART == Part::Two)
    {
        Part2(argc, argv);
    }
}