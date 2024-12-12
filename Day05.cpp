#include <chrono>
#include <fstream>
#include <iostream>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
    std::unordered_map<int, std::unordered_set<int>> dependencies;

    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long ans = 0;

    bool readRules = true;

    while (std::getline(inFile, line))
    {
        if (line == "")
        {
            readRules = false;
            continue;
        }

        if (readRules)
        {
            auto s = Split(line, "|");
            dependencies[std::stoi(s[1])].emplace(std::stoi(s[0]));
        }
        else
        {
            auto s = Split(line, ",");
            std::vector<int> pages(s.size());
            std::transform(s.begin(), s.end(), pages.begin(), [](std::string &num) { return std::stoi(num); });
            std::unordered_map<int, bool> printed;

            bool isValid = true;
            for (auto page : pages)
            {

                if (dependencies.find(page) == dependencies.end())
                {
                    printed[page] = true;
                    continue;
                }
                for (auto dPage : dependencies[page])
                {
                    if (printed[dPage] == false && std::find(pages.begin(), pages.end(), dPage) != pages.end())
                    {
                        isValid = false;
                        goto done;
                    }
                }
                printed[page] = true;
            }
        done:
            if (isValid)
            {
                int mid = pages.size() / 2;
                ans += pages[mid];
            }
        }
    }
    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::unordered_map<int, std::unordered_set<int>> dependencies;

    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long ans = 0;

    bool readRules = true;

    while (std::getline(inFile, line))
    {
        if (line == "")
        {
            readRules = false;
            continue;
        }

        if (readRules)
        {
            auto s = Split(line, "|");
            dependencies[std::stoi(s[1])].emplace(std::stoi(s[0]));
        }
        else
        {
            auto s = Split(line, ",");
            std::vector<int> pages(s.size());
            std::transform(s.begin(), s.end(), pages.begin(), [](std::string &num) { return std::stoi(num); });
            std::unordered_map<int, bool> printed;

            bool isValid = true;
            for (auto page : pages)
            {

                if (dependencies.find(page) == dependencies.end())
                {
                    printed[page] = true;
                    continue;
                }
                for (auto dPage : dependencies[page])
                {
                    if (printed[dPage] == false && std::find(pages.begin(), pages.end(), dPage) != pages.end())
                    {
                        isValid = false;
                        goto done;
                    }
                }
                printed[page] = true;
            }
        done:
            if (!isValid)
            {
                std::vector<int> sortedPages;
                std::vector<int> unsortedPages;
                for (auto page : pages)
                {
                    if (printed[page])
                    {
                        sortedPages.emplace_back(page);
                    }
                    else
                    {
                        unsortedPages.emplace_back(page);
                    }
                }
                while (!unsortedPages.empty())
                {
                    for (auto itr = unsortedPages.begin(); itr != unsortedPages.end(); itr++)
                    {
                        bool canBePrinted = true;
                        if (dependencies.find(*itr) == dependencies.end())
                        {
                            printed[*itr] = true;
                            sortedPages.emplace_back(*itr);
                            unsortedPages.erase(itr);
                            break;
                        }
                        for (auto dPage : dependencies[*itr])
                        {
                            if (printed[dPage] == false && std::find(pages.begin(), pages.end(), dPage) != pages.end())
                            {
                                canBePrinted = false;
                                break;
                            }
                        }
                        if (canBePrinted)
                        {
                            printed[*itr] = true;
                            sortedPages.emplace_back(*itr);
                            unsortedPages.erase(itr);
                            break;
                        }
                    }
                }
                int mid = sortedPages.size() / 2;
                ans += sortedPages[mid];
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