#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
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
constexpr Part PART = Part::One;

struct BlockRegion
{
    long long id;
    long long size;
};

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::vector<BlockRegion> memory;

    std::getline(inFile, line);
    bool isFile = true;
    int id = 0;
    for (auto c : line)
    {
        if (isFile)
        {
            memory.emplace_back(id++, c - '0');
        }
        else
        {
            memory.emplace_back(-1, c - '0');
        }
        isFile = !isFile;
    }
    int freeIdx = 1;
    int lastFileIdx = (memory.back().id != -1) ? memory.size() - 1 : memory.size() - 2;
    while (freeIdx < lastFileIdx)
    {
        if (memory[freeIdx].size == 0)
        {
            freeIdx += 2;
        }
        else if (memory[freeIdx].size < memory[lastFileIdx].size)
        {
            memory[freeIdx].id = memory[lastFileIdx].id;
            memory.emplace(memory.begin() + lastFileIdx + 1, -1, memory[freeIdx].size);
            memory[lastFileIdx].size -= memory[freeIdx].size;
            freeIdx += 2;
        }
        else if (memory[freeIdx].size == memory[lastFileIdx].size)
        {
            memory[freeIdx].id = memory[lastFileIdx].id;
            memory[lastFileIdx].id = -1;

            freeIdx += 2;
            lastFileIdx -= 2;
        }
        else
        {
            memory[freeIdx].id = memory[lastFileIdx].id;
            memory.emplace(memory.begin() + freeIdx + 1, -1, memory[freeIdx].size - memory[lastFileIdx].size);
            lastFileIdx++;
            memory[freeIdx].size = memory[lastFileIdx].size;
            memory[lastFileIdx].id = -1;

            freeIdx++;
            lastFileIdx -= 2;
        }
    }

    long long idx = 0;
    for (auto &b : memory)
    {
        if (b.size == 0)
        {
            continue;
        }
        if (b.id >= 0)
        {
            unsigned long long sumIds = ((idx + b.size - 1) * (idx + b.size) - (idx - 1) * idx) / 2;
            ans += sumIds * b.id;
        }
        idx += b.size;
    }

    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::vector<BlockRegion> memory;
    std::set<int> freeMemory;

    std::getline(inFile, line);
    bool isFile = true;
    int id = 0;
    for (auto c : line)
    {
        if (isFile)
        {
            memory.emplace_back(id++, c - '0');
        }
        else
        {
            memory.emplace_back(-1, c - '0');
            freeMemory.emplace(memory.size() - 1);
        }
        isFile = !isFile;
    }

    int offset = 0;
    for (int i = memory.size() - 1; i >= 0; i--)
    {
        if (memory[i].id > 0)
        {
            for (auto itr = freeMemory.begin(); itr != freeMemory.end(); itr++)
            {
                int freeIdx = *itr;
                if (freeIdx >= i)
                {
                    break;
                }

                if (memory[freeIdx].size == memory[i].size)
                {
                    memory[freeIdx].id = memory[i].id;
                    memory[i].id = -1;
                    itr = freeMemory.erase(itr);
                    freeMemory.insert(i);
                    break;
                }
                else if (memory[freeIdx].size > memory[i].size)
                {
                    memory[freeIdx].id = memory[i].id;
                    memory.emplace(memory.begin() + freeIdx + 1, -1, memory[freeIdx].size - memory[i].size);
                    for (auto it2 = itr; it2 != freeMemory.end(); it2++)
                    {
                        auto v = *it2;
                        it2 = freeMemory.erase(it2);
                        freeMemory.insert(v + 1);
                    }
                    i++;
                    memory[freeIdx].size = memory[i].size;
                    memory[i].id = -1;
                    itr = freeMemory.erase(itr);
                    freeMemory.insert(freeIdx + 1);
                    freeMemory.insert(i);
                    break;
                }
            }
        }
    }

    long long idx = 0;
    for (auto &b : memory)
    {
        if (b.size == 0)
        {
            continue;
        }
        if (b.id >= 0)
        {
            unsigned long long sumIds = ((idx + b.size - 1) * (idx + b.size) - (idx - 1) * idx) / 2;
            ans += sumIds * b.id;
        }
        idx += b.size;
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