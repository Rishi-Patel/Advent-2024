#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

#include "Utils.h"

constexpr auto ENABLE_MEMO = true;

constexpr auto MAX_FWD_STEPS = 10;
constexpr auto MIN_FWD_STEPS = 4;
constexpr auto FWD_STEPS_MEMO = MAX_FWD_STEPS - MIN_FWD_STEPS + 1;

enum Part
{
    One,
    Two
};
constexpr Part PART = Part::One;

enum Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3
};

constexpr std::array<Direction, 4> DIRECTIONS = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};

constexpr std::array<std::array<unsigned int, FWD_STEPS_MEMO>, 4> GetDefaultEntry()
{
    std::array<std::array<unsigned int, FWD_STEPS_MEMO>, 4> defaultEntry;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < FWD_STEPS_MEMO; j++)
        {
            defaultEntry[i][j] = UINT_MAX;
        }
    }
    return defaultEntry;
}

constexpr std::array<std::array<bool, FWD_STEPS_MEMO>, 4> GetDefaultVisited()
{
    std::array<std::array<bool, FWD_STEPS_MEMO>, 4> defaultEntry;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < FWD_STEPS_MEMO; j++)
        {
            defaultEntry[i][j] = false;
        }
    }
    return defaultEntry;
}

std::pair<int, int> GetNextCoord(int x, int y, Direction d, bool turning)
{
    int steps = (turning) ? MIN_FWD_STEPS : 1;
    switch (d)
    {
    case Direction::Up:
        return {x, y - steps};
    case Direction::Right:
        return {x + steps, y};
    case Direction::Down:
        return {x, y + steps};
    case Direction::Left:
        return {x - steps, y};
    }
}

char GetChar(Direction d)
{
    switch (d)
    {
    case Direction::Up:
        return '^';
    case Direction::Right:
        return '>';
    case Direction::Down:
        return 'V';
    case Direction::Left:
        return '<';
    default:
        return 'X';
    }
}

Direction GetOppositeDirection(Direction d)
{
    switch (d)
    {
    case Direction::Up:
        return Direction::Down;
    case Direction::Right:
        return Direction::Left;
    case Direction::Down:
        return Direction::Up;
    case Direction::Left:
        return Direction::Right;
    }
}

struct Work
{
    int x, y;
    Direction d;
    unsigned int fwdSteps;
    unsigned int heat;
    std::string path;

    bool operator<(const Work &w) const
    {
        return heat > w.heat;
    }
};

bool CheckBounds(int x, int y, const std::vector<std::vector<int>> &map, bool turning)
{

    return (0 <= x && x < map.back().size() && 0 <= y && y < map.size()); // &&
    // // Part 2 check, if we turned we have to move at least MIN_FWD_STEPS, so if there isnt enough remaining
    // // steps, this isnt valid
    // (!turning || (x + MIN_FWD_STEPS < map.back().size() && y + MIN_FWD_STEPS < map.size())));
}

bool CheckMemo(const Work &w, std::vector<std::vector<std::array<std::array<unsigned int, FWD_STEPS_MEMO>, 4>>> &memo,
               std::vector<std::vector<std::array<std::array<bool, FWD_STEPS_MEMO>, 4>>> &visited)
{
    if constexpr (ENABLE_MEMO)
    {

        if (visited[w.y][w.x][w.d][w.fwdSteps] == false)
        {
            for (int fwdSteps = 0; fwdSteps <= static_cast<int>(w.fwdSteps); fwdSteps++)
            {
                if (memo[w.y][w.x][w.d][fwdSteps] >= w.heat)
                {
                    memo[w.y][w.x][w.d][fwdSteps] = w.heat;
                }
            }

            visited[w.y][w.x][static_cast<int>(w.d)][w.fwdSteps] = true;
            return true;
        }
        return false;
    }
    else
    {
        return true;
    }
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;
    std::vector<std::vector<int>> heatMap;
    std::vector<std::vector<std::array<std::array<unsigned int, FWD_STEPS_MEMO>, 4>>> memo;
    std::vector<std::vector<std::array<std::array<bool, FWD_STEPS_MEMO>, 4>>> visited;

    while (std::getline(inFile, line))
    {
        heatMap.emplace_back(std::vector<int>());
        memo.emplace_back(std::vector<std::array<std::array<unsigned int, FWD_STEPS_MEMO>, 4>>());
        visited.emplace_back(std::vector<std::array<std::array<bool, FWD_STEPS_MEMO>, 4>>());
        for (auto c : line)
        {
            memo.back().emplace_back(GetDefaultEntry());
            visited.back().emplace_back(GetDefaultVisited());
            heatMap.back().emplace_back(c - '0');
        }
    }

    unsigned int ans = UINT_MAX;
    std::priority_queue<Work> toDo;

    Work w1 = {MIN_FWD_STEPS, 0, Direction::Right, MAX_FWD_STEPS - MIN_FWD_STEPS, 0, ""};
    Work w2 = {0, MIN_FWD_STEPS, Direction::Down, MAX_FWD_STEPS - MIN_FWD_STEPS, 0, ""};
    int pathsProcessed{};
    int nodesProcessed{};

    memo[0][0][1][FWD_STEPS_MEMO - 1] = 0;
    memo[0][0][2][FWD_STEPS_MEMO - 1] = 0;
    for (int i = 1; i < MIN_FWD_STEPS; i++)
    {
        w1.heat += heatMap[0][i];
        w2.heat += heatMap[i][0];
    }
    toDo.push(w1);
    toDo.push(w2);

    while (!toDo.empty())
    {
        auto work = toDo.top();
        nodesProcessed++;
        toDo.pop();
        if (work.heat >= ans)
        {
            pathsProcessed++;
            continue;
        }
        if (work.x == heatMap[0].size() - 1 && work.y == heatMap.size() - 1)
        {
            ans = std::min(ans, work.heat + heatMap[work.y][work.x]);
            std::cout << work.path << "\n\n";
            pathsProcessed++;
            continue;
        }

        bool endOfPath = true;
        for (auto d : DIRECTIONS)
        {
            if (GetOppositeDirection(work.d) == d)
            {
                continue;
            }

            bool turning = (d != work.d);
            auto [x, y] = GetNextCoord(work.x, work.y, d, turning);
            if (CheckBounds(x, y, heatMap, turning) == false)
            {
                continue;
            }

            Work w{x,
                   y,
                   d,
                   (work.d == d) ? work.fwdSteps - 1 : MAX_FWD_STEPS - MIN_FWD_STEPS,
                   work.heat + heatMap[work.y][work.x],
                   work.path + GetChar(w.d) + " "};
            // Travelling in same direction
            if (!turning && work.fwdSteps == 0)
            {
                continue;
            }

            if (turning)
            {
                for (int i = 1; i < MIN_FWD_STEPS; i++)
                {
                    switch (d)
                    {
                    case Direction::Up:
                        w.heat += heatMap[y + i][x];
                        continue;
                    case Direction::Right:
                        w.heat += heatMap[y][x - i];
                        continue;
                    case Direction::Down:
                        w.heat += heatMap[y - i][x];
                        continue;
                    case Direction::Left:
                        w.heat += heatMap[y][x + i];
                        continue;
                    }
                }
            }

            if (CheckMemo(w, memo, visited))
            {
                toDo.emplace(w);
                endOfPath = false;
            }
        }

        if (endOfPath)
        {
            pathsProcessed++;
        }
    }
    std::cout << "Paths Processed: " << pathsProcessed << "\nNodes Processed: " << nodesProcessed << '\n';
    std::cout << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    while (std::getline(inFile, line))
    {
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

/*
Paths Processed: 2317075
Nodes Processed: 3766912
Ans: 44
*/

/*
Paths Processed: 200
Nodes Processed: 444
44
1256
*/