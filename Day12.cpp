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

std::pair<unsigned int, unsigned int> FindRegion(std::vector<std::vector<char>> &map,
                                                 std::vector<std::vector<bool>> &visited, char c, int sx, int sy)
{
    std::deque<std::pair<int, int>> work;
    work.emplace_back(sx, sy);
    unsigned int area = 0;
    unsigned int perimeter = 0;
    while (!work.empty())
    {
        auto [i, j] = work.front();
        work.pop_front();
        if (i < 0 || j < 0 || i >= map.back().size() || j >= map.back().size() || map[j][i] != c || visited[j][i])
        {
            continue;
        }
        area += 1;
        int perimeterAdd = 0;
        if (i == 0 || map[j][i - 1] != map[j][i])
        {
            perimeterAdd++;
        }
        if (j == 0 || map[j - 1][i] != map[j][i])
        {
            perimeterAdd++;
        }
        if (i == map.back().size() - 1 || map[j][i + 1] != map[j][i])
        {
            perimeterAdd++;
        }
        if (j == map.size() - 1 || map[j + 1][i] != map[j][i])
        {
            perimeterAdd++;
        }
        work.emplace_back(i + 1, j);
        work.emplace_back(i, j + 1);
        work.emplace_back(i - 1, j);
        work.emplace_back(i, j - 1);
        perimeter += perimeterAdd;
        visited[j][i] = true;
    }
    return {area, perimeter};
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;
    std::vector<std::vector<char>> map;
    std::vector<std::vector<bool>> visited;
    std::unordered_map<char, std::pair<unsigned int, unsigned int>> vals;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<char>());
        visited.emplace_back(std::vector<bool>());
        for (char c : line)
        {
            map.back().emplace_back(c);
            visited.back().emplace_back(false);
        }
    }

    for (int j = 0; j < map.size(); j++)
    {
        for (int i = 0; i < map.back().size(); i++)
        {
            if (visited[j][i])
            {
                continue;
            }
            auto [area, perimeter] = FindRegion(map, visited, map[j][i], i, j);
            ans += area * perimeter;
        }
    }

    std::cout << ans << std::endl;
}

std::pair<unsigned int, unsigned int> FindRegionAndSides(std::vector<std::vector<char>> &map,
                                                         std::vector<std::vector<bool>> &visited, char c, int sx,
                                                         int sy)
{
    std::deque<std::pair<int, int>> work;
    work.emplace_back(sx, sy);
    unsigned int area = 0;
    unsigned int perimeter = 0;
    while (!work.empty())
    {
        auto [i, j] = work.front();
        work.pop_front();
        if (i < 0 || j < 0 || i >= map.back().size() || j >= map.back().size() || map[j][i] != c || visited[j][i])
        {
            continue;
        }
        area += 1;
        int perimeterAdd = 0;

        // Sides directly map to number of corners, consider this square:
        /*
           |---|
           |   |
           |___|
         */
        // For corners = four sides. More complicated examples follow this too trust me :)
        // There are 8 possible corners (4 external, four internal)

        // Check upper left corner ( |- )
        if ((i == 0 || map[j][i - 1] != map[j][i]) && (j == 0 || map[j - 1][i] != map[j][i]))
        {
            perimeterAdd++;
        }
        // Check upper right corner ( -| )
        if ((i == map.back().size() - 1 || map[j][i + 1] != map[j][i]) && (j == 0 || map[j - 1][i] != map[j][i]))
        {
            perimeterAdd++;
        }
        // Check upper bottom left corner ( |_ )
        if ((i == 0 || map[j][i - 1] != map[j][i]) && (j == map.size() - 1 || map[j + 1][i] != map[j][i]))
        {
            perimeterAdd++;
        }
        // Check upper bottom right corner ( _| )
        if ((i == map.back().size() - 1 || map[j][i + 1] != map[j][i]) &&
            (j == map.size() - 1 || map[j + 1][i] != map[j][i]))
        {
            perimeterAdd++;
        }

        // Check internal upper left corner ( |- )
        if ((i == 0 || map[j][i - 1] == map[j][i]) && (j == 0 || map[j - 1][i] == map[j][i]) && i != 0 && j != 0 &&
            map[j - 1][i - 1] != map[j][i])
        {
            perimeterAdd++;
        }
        // Check upper right corner ( -| )
        if ((i == map.back().size() - 1 || map[j][i + 1] == map[j][i]) && (j == 0 || map[j - 1][i] == map[j][i]) &&
            i != map.back().size() - 1 && j != 0 && map[j - 1][i + 1] != map[j][i])
        {
            perimeterAdd++;
        }
        // Check upper bottom left corner ( |_ )
        if ((i == 0 || map[j][i - 1] == map[j][i]) && (j == map.size() - 1 || map[j + 1][i] == map[j][i]) && i != 0 &&
            j != map.size() - 1 && map[j + 1][i - 1] != map[j][i])
        {
            perimeterAdd++;
        }
        // Check upper bottom right corner ( _| )
        if ((i == map.back().size() - 1 || map[j][i + 1] == map[j][i]) &&
            (j == map.size() - 1 || map[j + 1][i] == map[j][i]) && i != map.back().size() - 1 && j != map.size() - 1 &&
            map[j + 1][i + 1] != map[j][i])
        {
            perimeterAdd++;
        }

        work.emplace_back(i + 1, j);
        work.emplace_back(i, j + 1);
        work.emplace_back(i - 1, j);
        work.emplace_back(i, j - 1);
        perimeter += perimeterAdd;
        visited[j][i] = true;
    }
    return {area, perimeter};
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;
    std::vector<std::vector<char>> map;
    std::vector<std::vector<bool>> visited;
    std::unordered_map<char, std::pair<unsigned int, unsigned int>> vals;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<char>());
        visited.emplace_back(std::vector<bool>());
        for (char c : line)
        {
            map.back().emplace_back(c);
            visited.back().emplace_back(false);
        }
    }

    for (int j = 0; j < map.size(); j++)
    {
        for (int i = 0; i < map.back().size(); i++)
        {
            if (visited[j][i])
            {
                continue;
            }
            auto [area, perimeter] = FindRegionAndSides(map, visited, map[j][i], i, j);
            ans += area * perimeter;
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