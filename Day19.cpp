#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
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

enum class GearComponent
{
    X,
    M,
    A,
    S,
    Unknown
};
using Gear = std::unordered_map<GearComponent, std::pair<unsigned long long, unsigned long long>>;

GearComponent CharToGearComponent(char c)
{
    switch (c)
    {
    case 'x':
        return GearComponent::X;
    case 'm':
        return GearComponent::M;
    case 'a':
        return GearComponent::A;
    case 's':
        return GearComponent::S;
    }
}

enum class Condition
{
    Empty,
    Greater,
    Less
};
struct Rule
{
    GearComponent component;
    Condition condition;
    int val;
    std::string next;

    bool operator()(const Gear &gear)
    {
        switch (condition)
        {
        case Condition::Empty:
            return true;
        case Condition::Greater:
            return gear.at(component).first > val;
        case Condition::Less:
            return gear.at(component).second < val;
        }
    }

    std::vector<std::pair<Gear, bool>> GetGears(Gear &parentGear)
    {
        std::vector<std::pair<Gear, bool>> gears;
        switch (condition)
        {
        case Condition::Empty:
            return {{parentGear, true}};
        case Condition::Greater:
            if (parentGear.at(component).second <= val + 1)
            {
                return {{parentGear, false}};
            }
            else
            {
                Gear g1 = parentGear;
                g1.at(component).second = val + 1;
                Gear g2 = parentGear;
                g2.at(component).first = val + 1;
                return {{g1, false}, {g2, true}};
            }
        case Condition::Less:
            if (parentGear.at(component).first >= val)
            {
                return {{parentGear, false}};
            }
            else
            {
                Gear g1 = parentGear;
                g1.at(component).second = val;
                Gear g2 = parentGear;
                g2.at(component).first = val;
                return {{g1, true}, {g2, false}};
            }
        }
    }
};

class Workflow
{
  public:
    Workflow(std::string &&rulesStr)
    {
        int start = 0;
        int end = rulesStr.find(',', start);
        while (true)
        {
            auto ruleStr = rulesStr.substr(start, end - start);
            auto colonPos = ruleStr.find(':');
            if (colonPos == std::string::npos)
            {
                rules.emplace_back(GearComponent::Unknown, Condition::Empty, 0, ruleStr);
            }
            else
            {
                rules.emplace_back(CharToGearComponent(ruleStr[0]),
                                   ruleStr[1] == '>' ? Condition::Greater : Condition::Less,
                                   std::stoi(ruleStr.substr(2, colonPos - 2)), ruleStr.substr(colonPos + 1));
            }
            if (end == std::string::npos)
            {
                break;
            }
            start = end + 1;
            end = rulesStr.find(',', start);
        }
    }

    std::string RunWorkflow(Gear &g)
    {
        for (auto &rule : rules)
        {
            if (rule(g))
            {
                return rule.next;
            }
        }
        return "XXXXXX";
    }
    std::vector<Rule> rules;
};

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    std::unordered_map<std::string, Workflow> workflows;
    std::vector<Gear> gears;

    bool readWorkflow = true;
    while (std::getline(inFile, line))
    {
        if (line == "")
        {
            readWorkflow = false;
            continue;
        }
        if (readWorkflow)
        {
            auto curlPos = line.find('{');
            workflows.emplace(line.substr(0, curlPos),
                              Workflow(line.substr(curlPos + 1, line.find('}') - curlPos - 1)));
        }
        else
        {
            gears.emplace_back(Gear());
            auto start = 0;
            auto end = 0;
            for (int i = 0; i < 4; i++)
            {
                start = line.find('=', start) + 1;
                end = line.find(',', start);
                gears.back()[static_cast<GearComponent>(i)] = {std::stoi(line.substr(start, end - start)),
                                                               std::stoi(line.substr(start, end - start)) + 1};
            }
        }
    }
    long long ans = 0;
    for (auto gear : gears)
    {
        std::string workflowName = "in";
        while (workflowName != "A" && workflowName != "R")
        {
            workflowName = workflows.at(workflowName).RunWorkflow(gear);
        }
        if (workflowName == "A")
        {
            for (auto [_, val] : gear)
            {
                ans += ((val.second - val.first) * (val.first + val.second - 1)) / 2;
            }
        }
    }
    std::cout << ans << '\n';
}

struct Work
{
    Gear gear;
    std::string workflow;
    int rule;
};

std::string GearComponentToStr(GearComponent gc)
{
    switch (gc)
    {
    case GearComponent::X:
        return "X";
    case GearComponent::M:
        return "M";
    case GearComponent::A:
        return "A";
    case GearComponent::S:
        return "S";
    }
}

void PrintGear(Gear &g)
{
    for (auto [name, val] : g)
    {
        std::cout << GearComponentToStr(name) << ": [" << val.first << ", " << val.second << ")\n";
    }
    std::cout << '\n';
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    std::unordered_map<std::string, Workflow> workflows;
    while (std::getline(inFile, line))
    {
        if (line == "")
        {
            break;
        }

        auto curlPos = line.find('{');
        workflows.emplace(line.substr(0, curlPos), Workflow(line.substr(curlPos + 1, line.find('}') - curlPos - 1)));
    }

    std::queue<Work> toDo;
    toDo.emplace(Gear(), "in", 0);
    for (int i = 0; i < 4; i++)
    {
        toDo.back().gear[static_cast<GearComponent>(i)] = {1, 4001};
    }

    unsigned long long ans = 0;
    while (!toDo.empty())
    {
        auto [gear, workflowName, ruleNum] = toDo.front();
        toDo.pop();

        auto newGears = workflows.at(workflowName).rules[ruleNum].GetGears(gear);
        for (auto [nGear, passed] : newGears)
        {
            const auto &rule = workflows.at(workflowName).rules[ruleNum];
            if (passed)
            {
                if (rule.next == "A")
                {
                    // PrintGear(nGear);
                    unsigned long long numOptions = 1;
                    for (auto [_, val] : nGear)
                    {
                        numOptions *= (val.second - val.first);
                    }
                    // std::cout << numOptions << "\n\n";
                    ans += numOptions;
                }
                else if (rule.next == "R")
                {
                }
                else
                {
                    toDo.emplace(nGear, rule.next, 0);
                }
            }
            else
            {
                toDo.emplace(nGear, workflowName, ruleNum + 1);
            }
        }
    }
    std::cout << ans << '\n';
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