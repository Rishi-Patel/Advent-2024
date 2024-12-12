#include <algorithm>
#include <chrono>
#include <deque>
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

int digitCount(const unsigned long long &value)
{
    int digits = 0;
    unsigned long long temp = value;
    while (temp > 0)
    {
        digits++;
        temp /= 10;
    }
    return digits;
}

bool evenDigits(const unsigned long long &value)
{
    return digitCount(value) % 2 == 0;
}

std::pair<unsigned long long, unsigned long long> splitEven(const unsigned long long &value)
{
    int halfWidth = digitCount(value) / 2;
    unsigned long long rightVal = 0;
    unsigned long long leftVal = value;
    unsigned long long place = 1;
    for (int i = 0; i < halfWidth; ++i)
    {
        int digit = leftVal % 10;
        leftVal /= 10;
        rightVal += place * digit;
        place *= 10;
    }

    return {leftVal, rightVal};
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::getline(inFile, line);
    auto s = Split(line, " ");

    std::vector<unsigned long long> stones(s.size());
    std::transform(s.begin(), s.end(), stones.begin(), [](std::string &num) { return std::stoull(num); });

    int numBlinks = 25;

    std::vector<unsigned long long> next_stones;
    for (int i = 0; i < numBlinks; i++)
    {
        next_stones.clear();
        for (auto stone : stones)
        {
            if (stone == 0)
            {
                next_stones.emplace_back(1);
                continue;
            }
            std::string stone_str = std::to_string(stone);
            if (stone_str.size() % 2 == 0)
            {
                auto mid = stone_str.size() / 2;
                auto left_str = stone_str.substr(0, mid);
                auto right_str = stone_str.substr(mid);
                next_stones.emplace_back(std::stoull(left_str));
                next_stones.emplace_back(std::stoull(right_str));
            }
            else
            {
                next_stones.emplace_back(stone * 2024);
            }
        }
        stones = next_stones;
    }
    ans = stones.size();
    std::cout << ans << std::endl;
}

// {Stone #, currentStep}
using Key = std::pair<unsigned long long, unsigned int>;

struct KeyHash
{
    // Stole hash from somewhere online :)
    size_t operator()(const Key &k) const
    {
        size_t hash1 = std::hash<unsigned long long>{}(k.first);
        size_t hash2 = std::hash<unsigned int>{}(k.second);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;
    int numBlinks = 75;

    std::getline(inFile, line);
    auto s = Split(line, " ");

    std::deque<Key> all_stones;
    for (std::string str : s)
    {
        all_stones.emplace_back(std::stoull(str), numBlinks);
    }

    std::unordered_map<Key, unsigned long long, KeyHash> cache;

    while (!all_stones.empty())
    {
        auto [stoneNum, stepsLeft] = all_stones.front();
        all_stones.pop_front();
        if (cache.contains({stoneNum, stepsLeft}))
        {
            ans += cache[{stoneNum, stepsLeft}];
            continue;
        }

        if (stepsLeft == 0)
        {
            cache[{stoneNum, stepsLeft}] = 1;
            ans++;
            continue;
        }
        if (stoneNum == 0)
        {
            if (cache.find({1, stepsLeft - 1}) != cache.end())
            {
                ans += cache[{1, stepsLeft - 1}];
                cache[{stoneNum, stepsLeft}] = cache[{1, stepsLeft - 1}];
            }
            else
            {
                all_stones.push_front({1, stepsLeft - 1});
            }
            continue;
        }
        if (evenDigits(stoneNum))
        {
            auto [left, right] = splitEven(stoneNum);
            bool leftCached = false;
            bool rightCached = false;
            if (cache.find({left, stepsLeft - 1}) != cache.end())
            {
                ans += cache[{left, stepsLeft - 1}];
                leftCached = true;
            }
            else
            {
                all_stones.push_front({left, stepsLeft - 1});
            }

            if (cache.find({right, stepsLeft - 1}) != cache.end())
            {
                ans += cache[{right, stepsLeft - 1}];
                rightCached = true;
            }
            else
            {
                all_stones.push_front({right, stepsLeft - 1});
            }

            if (leftCached && rightCached)
            {
                cache[{stoneNum, stepsLeft}] = cache[{left, stepsLeft - 1}] + cache[{right, stepsLeft - 1}];
            }
        }
        else
        {
            if (cache.find({stoneNum * 2024, stepsLeft - 1}) != cache.end())
            {
                ans += cache[{stoneNum * 2024, stepsLeft - 1}];
                cache[{stoneNum, stepsLeft}] = cache[{stoneNum * 2024, stepsLeft - 1}];
            }
            else
            {
                all_stones.push_front({stoneNum * 2024, stepsLeft - 1});
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