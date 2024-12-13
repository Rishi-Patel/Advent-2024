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

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    while (std::getline(inFile, line))
    {
        if (line == "")
        {
            continue;
        }
        auto s = Split(line, ",");
        std::vector<int> A = {std::stoi(Split(s[0], "+")[1]), std::stoi(Split(s[1], "+")[1])};

        std::getline(inFile, line);
        s = Split(line, ",");
        std::vector<int> B = {std::stoi(Split(s[0], "+")[1]), std::stoi(Split(s[1], "+")[1])};

        std::getline(inFile, line);
        s = Split(line, ",");
        std::vector<int> Dest = {std::stoi(Split(s[0], "=")[1]), std::stoi(Split(s[1], "=")[1])};

        auto det = A[0] * B[1] - A[1] * B[0];
        if (det == 0)
        {
            std::cout << "No solution\n";
            continue;
        }

        auto A_tokens = (Dest[0] * B[1] - Dest[1] * B[0]) / det;
        auto B_tokens = (A[0] * Dest[1] - A[1] * Dest[0]) / det;

        // So solving the linear equations can lead to decimal answers for tokens which should be ignored for this
        // problem Two was I can think to solve this are 1) make the A/B_tokens calculations doubles and check if the
        // result is an int or just check if plugging in the rounded token values in the lienar equations give the
        // correct values.
        // Im choosing to do the latter solution, have a feeling double conversions may be slower? But more importantly
        // I dont wanna deal with double rounding and other issues. Can confirm perfromance later.
        if (A_tokens * A[0] + B_tokens * B[0] == Dest[0] && A_tokens * A[1] + B_tokens * B[1] == Dest[1])
        {
            std::cout << A_tokens << " " << B_tokens << std::endl;
            ans += 3 * A_tokens + 1 * B_tokens;
        }
        else
        {
            std::cout << "No integer solution\n";
        }
    }

    std::cout << std::endl << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    while (std::getline(inFile, line))
    {
        if (line == "")
        {
            continue;
        }
        auto s = Split(line, ",");
        std::vector<long long> A = {std::stoll(Split(s[0], "+")[1]), std::stoll(Split(s[1], "+")[1])};

        std::getline(inFile, line);
        s = Split(line, ",");
        std::vector<long long> B = {std::stoll(Split(s[0], "+")[1]), std::stoll(Split(s[1], "+")[1])};

        std::getline(inFile, line);
        s = Split(line, ",");
        std::vector<long long> Dest = {10000000000000LL + std::stoll(Split(s[0], "=")[1]),
                                       10000000000000LL + std::stoll(Split(s[1], "=")[1])};

        long long det = A[0] * B[1] - A[1] * B[0];
        if (det == 0)
        {
            std::cout << "No solution\n";
            continue;
        }

        long long A_tokens = (Dest[0] * B[1] - Dest[1] * B[0]) / det;
        long long B_tokens = (A[0] * Dest[1] - A[1] * Dest[0]) / det;

        // So solving the linear equations can lead to decimal answers for tokens which should be ignored for this
        // problem Two was I can think to solve this are 1) make the A/B_tokens calculations doubles and check if the
        // result is an int or just check if plugging in the rounded token values in the lienar equations give the
        // correct values.
        // Im choosing to do the latter solution, have a feeling double conversions may be slower? But more importantly
        // I dont wanna deal with double rounding and other issues. Can confirm perfromance later.
        if (A_tokens * A[0] + B_tokens * B[0] == Dest[0] && A_tokens * A[1] + B_tokens * B[1] == Dest[1])
        {
            std::cout << A_tokens << " " << B_tokens << std::endl;
            ans += 3 * A_tokens + 1 * B_tokens;
        }
        else
        {
            std::cout << "No integer solution\n";
        }
    }

    std::cout << std::endl << ans << std::endl;
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