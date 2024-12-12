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

void rotateRight(int &dx, int &dy)
{
    if (dx != 0)
    {
        dy = dx;
        dx = 0;
        return;
    }
    dx = -dy;
    dy = 0;
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long ans = 0;

    std::vector<std::vector<bool>> map;
    std::vector<std::vector<bool>> visited;
    int gx, gy;
    int dx, dy;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<bool>());
        visited.emplace_back(std::vector<bool>());
        for (auto c : line)
        {
            visited.back().emplace_back(false);
            switch (c)
            {
            case '.':
                map.back().emplace_back(false);
                break;
            case '#':
                map.back().emplace_back(true);
                break;
            case '^':
                map.back().emplace_back(false);
                gx = map.back().size() - 1;
                gy = map.size() - 1;
                dx = 0;
                dy = -1;
                break;
            }
        }
    }
    int steps = 0;
    while (true)
    {
        if (visited[gy][gx] == false)
        {
            visited[gy][gx] = true;
            ans++;
        }

        int n_gx = gx + dx;
        int n_gy = gy + dy;
        if (n_gx < 0 || n_gx >= map.back().size() || n_gy < 0 || n_gy >= map.size())
        {
            std::cout << "Step: " << steps << std::endl;
            std::cout << ans << std::endl;
            return;
        }

        while (map[n_gy][n_gx])
        {
            rotateRight(dx, dy);
            n_gx = gx + dx;
            n_gy = gy + dy;
        }

        gx = n_gx;
        gy = n_gy;
        steps++;
    }
}

int DxDyToDir(int dx, int dy)
{
    if (dx == 0 && dy == -1)
    {
        return 0;
    }
    if (dx == 1 && dy == 0)
    {
        return 1;
    }
    if (dx == 0 && dy == 1)
    {
        return 2;
    }
    if (dx == -1 && dy == 0)
    {
        return 3;
    }
    return -1;
}

bool checkForLoop(const std::vector<std::vector<bool>> &map, int gx, int gy, int dx, int dy)
{
    // std::array<bool, 4> prototype = {false, false, false, false};
    std::vector<std::vector<std::vector<bool>>> visited(
        map.size(), (std::vector<std::vector<bool>>(map.back().size(), std::vector<bool>(4, false))));

    while (true)
    {
        if (visited[gy][gx][DxDyToDir(dx, dy)] == true)
        {
            return true;
        }
        else
        {
            visited[gy][gx][DxDyToDir(dx, dy)] = true;
        }

        int n_gx = gx + dx;
        int n_gy = gy + dy;
        if (n_gx < 0 || n_gx >= map.back().size() || n_gy < 0 || n_gy >= map.size())
        {
            return false;
        }

        while (map[n_gy][n_gx])
        {
            rotateRight(dx, dy);
            n_gx = gx + dx;
            n_gy = gy + dy;
        }

        gx = n_gx;
        gy = n_gy;
    }
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long ans = 0;

    std::vector<std::vector<bool>> map;
    std::vector<std::vector<bool>> placedBefore;
    int gx, gy;
    int sx, sy;
    int dx, dy;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<bool>());
        placedBefore.emplace_back(std::vector<bool>());
        for (auto c : line)
        {
            placedBefore.back().emplace_back(false);
            switch (c)
            {
            case '.':
                map.back().emplace_back(false);
                break;
            case '#':
                map.back().emplace_back(true);
                break;
            case '^':
                map.back().emplace_back(false);
                gx = map.back().size() - 1;
                gy = map.size() - 1;
                dx = 0;
                dy = -1;
                break;
            }
        }
    }
    int steps = 0;
    sx = gx;
    sy = gy;
    while (true)
    {
        int n_gx = gx + dx;
        int n_gy = gy + dy;
        if (n_gx < 0 || n_gx >= map.back().size() || n_gy < 0 || n_gy >= map.size())
        {
            std::cout << "Step: " << steps << std::endl;
            std::cout << ans << std::endl;
            return;
        }

        while (map[n_gy][n_gx])
        {
            rotateRight(dx, dy);
            n_gx = gx + dx;
            n_gy = gy + dy;
        }
        if (placedBefore[n_gy][n_gx] == false)
        {
            map[n_gy][n_gx] = true;
            ans += (checkForLoop(map, gx, gy, dx, dy)) ? 1 : 0;
            map[n_gy][n_gx] = false;
            placedBefore[n_gy][n_gx] = true;
        }

        gx = n_gx;
        gy = n_gy;
        steps++;
        if (steps % 32 == 0)
        {
            std::cout << "Step: " << steps << std::endl;
        }
    }
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