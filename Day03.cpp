#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
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

    std::regex r("mul\\((\\d+),(\\d+)\\)");
    while (std::getline(inFile, line))
    {
        std::sregex_iterator iter(line.begin(), line.end(), r);
        std::sregex_iterator end;

        while (iter != end)
        {
            ans += std::stoi((*iter)[1]) * std::stoi((*iter)[2]);
            ++iter;
        }
    }

    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    int ans = 0;
    bool enabled = true;

    std::regex r("do\\(\\)|mul\\((\\d+),(\\d+)\\)|don't\\(\\)");
    while (std::getline(inFile, line))
    {
        std::sregex_iterator iter(line.begin(), line.end(), r);
        std::sregex_iterator end;

        while (iter != end)
        {
            // Each match is 3 captures:
            // First match is whole string, either: "Mul(x,y)" or "do()" or "dont't()"
            // Second and third matches are the numbers in Mul() or two empty captures for do/don't()
            for (unsigned i = 0; i < iter->size(); i += 3)
            {
                if ((*iter)[i] == "do()")
                {
                    enabled = true;
                }
                else if ((*iter)[i] == "don't()")
                {
                    enabled = false;
                }
                else
                {
                    ans += (enabled) ? std::stoi((*iter)[1]) * std::stoi((*iter)[2]) : 0;
                }
            }
            ++iter;
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