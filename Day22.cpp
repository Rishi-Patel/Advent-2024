#include "Utils.h"
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>

enum Part
{
    One,
    Two
};
constexpr Part PART = Part::One;

enum class Axis
{
    X,
    Y,
    Z
};

struct Brick
{
    int x, y, z;
    int length;
    Axis axis;
    int maxHeight;
    bool canRemove;
};

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    unsigned long long ans = 0;
    std::vector<Brick> bricks;

    int maxX, maxY = 0;

    while (std::getline(inFile, line))
    {
        auto split = line.find('~');
        auto start = Split(line.substr(0, split), ",");
        auto end = Split(line.substr(split + 1), ",");

        if (std::stoi(end[2]) < std::stoi(start[2]))
        {
            std::swap(start, end);
        }

        maxX = std::max(std::max(std::stoi(start[0]), std::stoi(end[0])), maxX);
        maxY = std::max(std::max(std::stoi(start[1]), std::stoi(end[1])), maxY);

        Axis a = Axis::X;
        int length = 0;
        if (start[0] != end[0])
        {
            a = Axis::X;
            length = std::abs(std::stoi(start[0]) - std::stoi(end[0]));
        }
        else if (start[1] != end[1])
        {
            a = Axis::Y;
            length = std::abs(std::stoi(start[1]) - std::stoi(end[1]));
        }
        else if (start[2] != end[2])
        {
            a = Axis::Z;
            length = std::abs(std::stoi(start[2]) - std::stoi(end[2]));
        }
        bricks.emplace_back(std::stoi(start[0]), std::stoi(start[1]), std::stoi(start[2]), length, a, std::stoi(end[2]),
                            true);
    }
    std::sort(bricks.begin(), bricks.end(), [](const Brick &a, const Brick &b) { return a.z < b.z; });

    std::vector<std::vector<int>> heightMap = std::vector<std::vector<int>>(maxY + 1, std::vector<int>(maxX + 1, -1));

    for (int i = 0; i < bricks.size(); i++)
    {
        int height = 1;
        int brickUnder = -1;
        bool multipleUnder = false;
        switch (bricks[i].axis)
        {
        case Axis::X:
            for (int j = bricks[i].x; j <= bricks[i].x + bricks[i].length; j++)
            {
                if (heightMap[bricks[i].y][j] != -1)
                {
                    if (height < bricks[heightMap[bricks[i].y][j]].maxHeight + 1)
                    {
                        height = bricks[heightMap[bricks[i].y][j]].maxHeight + 1;
                        brickUnder = heightMap[bricks[i].y][j];
                        multipleUnder = false;
                    }
                    else if (height == bricks[heightMap[bricks[i].y][j]].maxHeight + 1)
                    {
                        multipleUnder = true;
                    }
                }
            }
            bricks[i].maxHeight = height;
            if (brickUnder != -1 && !multipleUnder)
            {
                bricks[brickUnder].canRemove = false;
            }
            for (int j = bricks[i].x; j <= bricks[i].x + bricks[i].length; j++)
            {
                heightMap[bricks[i].y][j] = i;
            }

            break;
        case Axis::Y:
            for (int j = bricks[i].y; j <= bricks[i].y + bricks[i].length; j++)
            {
                if (heightMap[j][bricks[i].x] != -1)
                {
                    if (height < bricks[heightMap[j][bricks[i].x]].maxHeight + 1)
                    {
                        height = bricks[heightMap[j][bricks[i].x]].maxHeight + 1;
                        multipleUnder = false;
                    }
                    else if (height == bricks[heightMap[j][bricks[i].x]].maxHeight + 1)
                    {
                        multipleUnder = true;
                    }
                }
            }
            bricks[i].maxHeight = height;
            if (brickUnder != -1 && !multipleUnder)
            {
                bricks[brickUnder].canRemove = false;
            }
            for (int j = bricks[i].y; j <= bricks[i].y + bricks[i].length; j++)
            {
                heightMap[j][bricks[i].x] = i;
            }

            break;
        case Axis::Z:
            if (heightMap[bricks[i].y][bricks[i].x] != -1)
            {
                bricks[heightMap[bricks[i].y][bricks[i].x]].canRemove = false;
                height = bricks[heightMap[bricks[i].y][bricks[i].x]].maxHeight + 1;
            }
            heightMap[bricks[i].y][bricks[i].x] = i;
            bricks[i].maxHeight = height + bricks[i].length;
            break;
        }
    }

    for (const auto &brick : bricks)
    {
        if (brick.canRemove)
        {
            ans++;
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