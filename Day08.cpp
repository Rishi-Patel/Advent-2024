#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
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
constexpr Part PART = Part::Two;

std::vector<std::pair<unsigned int, unsigned int>> GetAntinodes(std::pair<unsigned int, unsigned int> &antenna1,
                                                                std::pair<unsigned int, unsigned int> &antenna2)
{
    return {
        {(antenna1.first - antenna2.first) + antenna1.first, (antenna1.second - antenna2.second) + antenna1.second},
        {(antenna2.first - antenna1.first) + antenna2.first, (antenna2.second - antenna1.second) + antenna2.second}};
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::unordered_map<char, std::vector<std::pair<unsigned int, unsigned int>>> antennas;
    std::vector<std::vector<bool>> antinodes;

    while (std::getline(inFile, line))
    {
        antinodes.emplace_back(std::vector<bool>());
        for (char c : line)
        {
            antinodes.back().emplace_back(false);
            if (c != '.')
            {
                antennas[c].emplace_back(antinodes.back().size() - 1, antinodes.size() - 1);
            }
        }
    }

    for (auto &[antennaFreq, antennaLocs] : antennas)
    {
        for (int i = 0; i < antennaLocs.size(); i++)
        {
            for (int j = i + 1; j < antennaLocs.size(); j++)
            {
                auto &a1 = antennaLocs[i];
                auto &a2 = antennaLocs[j];
                auto anodes = GetAntinodes(antennaLocs[i], antennaLocs[j]);
                for (auto [x, y] : anodes)
                {
                    if (!(x < 0 || y < 0 || x >= antinodes.back().size() || y >= antinodes.size()))
                    {
                        if (!antinodes[y][x])
                        {
                            ans++;
                            antinodes[y][x] = true;
                        }
                    }
                }
            }
        }
    }

    std::cout << ans << std::endl;
}

std::vector<std::pair<unsigned int, unsigned int>> GetAntinodes(std::pair<unsigned int, unsigned int> &antenna1,
                                                                std::pair<unsigned int, unsigned int> &antenna2,
                                                                std::pair<unsigned int, unsigned int> &&bounds)
{
    std::vector<std::pair<unsigned int, unsigned int>> steps = {
        {(antenna1.first - antenna2.first), (antenna1.second - antenna2.second)},
        {(antenna2.first - antenna1.first), (antenna2.second - antenna1.second)}};
    std::vector<std::pair<unsigned int, unsigned int>> starts = {antenna1, antenna2};

    std::vector<std::pair<unsigned int, unsigned int>> nodes;
    for (int i = 0; i < steps.size(); i++)
    {
        auto &[dx, dy] = steps[i];
        auto &[sx, sy] = starts[i];
        int numSteps = 0;
        while (!(sx + numSteps * dx < 0 || sy + numSteps * dy < 0 || sx + numSteps * dx >= bounds.first ||
                 sy + numSteps * dy >= bounds.second))
        {
            nodes.emplace_back(sx + numSteps * dx, sy + numSteps * dy);
            numSteps++;
        }
    }
    return nodes;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;

    std::unordered_map<char, std::vector<std::pair<unsigned int, unsigned int>>> antennas;
    std::vector<std::vector<bool>> antinodes;

    while (std::getline(inFile, line))
    {
        antinodes.emplace_back(std::vector<bool>());
        for (char c : line)
        {
            antinodes.back().emplace_back(false);
            if (c != '.')
            {
                antennas[c].emplace_back(antinodes.back().size() - 1, antinodes.size() - 1);
            }
        }
    }

    for (auto &[antennaFreq, antennaLocs] : antennas)
    {
        for (int i = 0; i < antennaLocs.size(); i++)
        {
            for (int j = i + 1; j < antennaLocs.size(); j++)
            {
                auto &a1 = antennaLocs[i];
                auto &a2 = antennaLocs[j];
                auto anodes = GetAntinodes(antennaLocs[i], antennaLocs[j], {antinodes.back().size(), antinodes.size()});
                for (auto [x, y] : anodes)
                {
                    if (!antinodes[y][x])
                    {
                        ans++;
                        antinodes[y][x] = true;
                    }
                }
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

    if (PART == Part::One)
    {
        Part1(argc, argv);
    }
    else if (PART == Part::Two)
    {
        Part2(argc, argv);
    }
}