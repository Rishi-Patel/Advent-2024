#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>

#include "Utils.h"

enum Part
{
    One,
    Two
};
constexpr Part PART = Part::Two;

// Really dont need both but set contains is more succinct than find on vec lol
const std::unordered_set<char> xmas_set({'X', 'M', 'A', 'S'});
const std::vector<char> xmas_vec({'X', 'M', 'A', 'S'});
const std::vector<std::vector<char>> xmas_vec_p2 = {
    {'M', 'S', 'M', 'S'}, {'M', 'M', 'S', 'S'}, {'S', 'S', 'M', 'M'}, {'S', 'M', 'S', 'M'}};

struct Work
{
    int i, j;
    int di, dj;
};

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    int ans = 0;
    std::vector<std::vector<char>> map;
    std::queue<Work> toDo;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<char>());
        for (auto c : line)
        {
            if (xmas_set.contains(c))
            {
                map.back().emplace_back(c);
            }
            else
            {
                map.back().emplace_back('.');
            }
            if (c == 'X')
            {
                int i = map.back().size() - 1;
                int j = map.size() - 1;
                toDo.emplace(i, j, -1, -1);
                toDo.emplace(i, j, 0, -1);
                toDo.emplace(i, j, 1, -1);
                toDo.emplace(i, j, -1, 0);
                toDo.emplace(i, j, 1, 0);
                toDo.emplace(i, j, -1, 1);
                toDo.emplace(i, j, 0, 1);
                toDo.emplace(i, j, 1, 1);
            }
        }
    }

    while (!toDo.empty())
    {
        auto work = toDo.front();
        toDo.pop();

        bool found = true;
        int i = work.i;
        int j = work.j;
        for (int letIdx = 1; letIdx < 4; letIdx++)
        {
            i += work.di;
            j += work.dj;
            if (i < 0 || j < 0 || i >= map[0].size() || j >= map.size())
            {
                found = false;
                break;
            }
            if (map[j][i] != xmas_vec[letIdx])
            {
                found = false;
                break;
            }
        }
        ans += (found) ? 1 : 0;
    }

    std::cout << std::endl << ans << std::endl;
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    int ans = 0;
    std::vector<std::vector<char>> map;
    std::queue<Work> toDo;

    while (std::getline(inFile, line))
    {
        map.emplace_back(std::vector<char>());
        for (auto c : line)
        {
            if (xmas_set.contains(c))
            {
                map.back().emplace_back(c);
            }
            else
            {
                map.back().emplace_back('.');
            }
            if (c == 'A')
            {
                int i = map.back().size() - 1;
                int j = map.size() - 1;
                toDo.emplace(i, j, 0, 0);
            }
        }
    }

    // We dont use di,dj in part 2
    while (!toDo.empty())
    {
        auto work = toDo.front();
        toDo.pop();

        for (const auto &config : xmas_vec_p2)
        {
            bool isValid = true;
            for (int dj = -1; dj < 2; dj += 2)
            {
                for (int di = -1; di < 2; di += 2)
                {
                    int i = work.i + di;
                    int j = work.j + dj;
                    if (i < 0 || j < 0 || i >= map[0].size() || j >= map.size())
                    {
                        isValid = false;
                        goto done;
                    }
                    int cIdx = ((di + 2 * dj) + 3) >> 1;
                    if (map[j][i] != config[cIdx])
                    {
                        isValid = false;
                        goto done;
                    }
                }
            }
        done:
            ans += (isValid) ? 1 : 0;
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

    if (PART == Part::One)
    {
        Part1(argc, argv);
    }
    else if (PART == Part::Two)
    {
        Part2(argc, argv);
    }
}