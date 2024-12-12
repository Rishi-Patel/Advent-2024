#include "Utils.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

enum Part
{
    One,
    Two
};
constexpr Part PART = Part::Two;

enum class Pulse
{
    High,
    Low
};

enum class ModuleType
{
    FlipFlop,
    Conjunction,
    Broadcast,
    UnTyped
};

struct Message
{
    std::string sender;
    std::string receiver;
    Pulse p;
};

class Module
{
  public:
    ModuleType type;
    std::string name;
    std::unordered_map<std::string, Module> *AllModules;
    std::vector<std::string> DestinationModules;
    std::unordered_map<std::string, Pulse> LastPulseReceived;
    std::queue<Message> receivedMessages;
    bool State;

    Module(ModuleType t, std::string &n, std::unordered_map<std::string, Module> *allModules,
           std::vector<std::string> &destinationModules)
        : type(t), name(n), AllModules(allModules), DestinationModules(destinationModules), State(false){};

    std::vector<Message> HandlePulse(Message message)
    {
        Pulse pulseToSend = Pulse::Low;
        switch (type)
        {
        case ModuleType::UnTyped:
            return {};
        case ModuleType::Broadcast:
            // "When [Broadcast] receives a pulse, it sends the same pulse to all of its destination modules."
            pulseToSend = message.p;
            break;
        case ModuleType::FlipFlop:
            /*
            If a flip-flop module receives a high pulse, it is ignored and nothing happens. However, if a flip-flop
            module receives a low pulse, it flips between on and off. If it was off, it turns on and sends a high pulse.
            If it was on, it turns off and sends a low pulse.
            */
            if (message.p == Pulse::High)
            {
                return {};
            }
            pulseToSend = (State == false) ? Pulse::High : Pulse::Low;
            State = !State;

            break;
        case ModuleType::Conjunction:
            /*
            Conjunction modules remember the type of the most recent pulse received from each of their connected
            input modules; they initially default to remembering a low pulse for each input. When a pulse is received,
            the conjunction module first updates its memory for that input. Then, if it remembers high pulses for all
            inputs, it sends a low pulse; otherwise, it sends a high pulse.
            */
            LastPulseReceived[message.sender] = message.p;
            bool allHigh = true;
            for (auto [_, lastPulse] : LastPulseReceived)
            {
                allHigh &= (lastPulse == Pulse::High);
            }
            pulseToSend = (allHigh) ? Pulse::Low : Pulse::High;
            break;
        }

        std::vector<Message> messages;
        for (const auto &modName : DestinationModules)
        {
            messages.emplace_back(name, modName, pulseToSend);
        }
        return messages;
    }
};

ModuleType GetModuleType(const std::string &name)
{
    if (name == "broadcaster")
    {
        return ModuleType::Broadcast;
    }
    if (name[0] == '%')
    {
        return ModuleType::FlipFlop;
    }
    if (name[0] == '&')
    {
        return ModuleType::Conjunction;
    }
    return ModuleType::UnTyped;
}

void Part1(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    std::unordered_map<std::string, Module> modules;

    while (std::getline(inFile, line))
    {
        auto name = line.substr(0, line.find(" ->"));
        auto type = GetModuleType(name);

        auto destinationModules = Split(line.substr(line.find(" -> ") + 4), ", ");
        auto n = (type != ModuleType::Broadcast) ? name.substr(1) : name;
        modules.emplace(n, Module(type, n, &modules, destinationModules));
    }

    for (auto &[n, m] : modules)
    {
        if (m.type == ModuleType::Conjunction)
        {
            for (auto &[_, m2] : modules)
            {
                bool isSender = false;
                for (auto receiver : m2.DestinationModules)
                {
                    isSender |= (receiver == m.name);
                }
                if (isSender)
                {
                    m.LastPulseReceived.emplace(m2.name, Pulse::Low);
                }
            }
        }
    }

    int numLow = 0;
    int numHigh = 0;
    constexpr auto NUM_PRESSES = 1000;
    std::queue<Message> toSend;
    for (int i = 0; i < NUM_PRESSES; i++)
    {
        toSend.emplace("button", "broadcaster", Pulse::Low);
        while (!toSend.empty())
        {
            Message m = toSend.front();
            toSend.pop();
            (m.p == Pulse::High) ? numHigh++ : numLow++;
            if (modules.find(m.receiver) == modules.end())
            {
                continue;
            }
            auto nextMessages = modules.at(m.receiver).HandlePulse(m);
            for (const auto &nextMessage : nextMessages)
            {
                toSend.push(nextMessage);
            }
        }
    }
    long long ans = numLow * numHigh;

    std::cout << ans << '\n';
}

void Part2(int argc, char **argv)
{
    std::ifstream inFile(argv[1]);
    std::string line;

    std::unordered_map<std::string, Module> modules;

    while (std::getline(inFile, line))
    {
        auto name = line.substr(0, line.find(" ->"));
        auto type = GetModuleType(name);

        auto destinationModules = Split(line.substr(line.find(" -> ") + 4), ", ");
        auto n = (type != ModuleType::Broadcast) ? name.substr(1) : name;
        modules.emplace(n, Module(type, n, &modules, destinationModules));
    }

    for (auto &[n, m] : modules)
    {
        if (m.type == ModuleType::Conjunction)
        {
            for (auto &[_, m2] : modules)
            {
                bool isSender = false;
                for (auto receiver : m2.DestinationModules)
                {
                    isSender |= (receiver == m.name);
                }
                if (isSender)
                {
                    m.LastPulseReceived.emplace(m2.name, Pulse::Low);
                }
            }
        }
    }

    std::queue<Message> toSend;
    int i = 1;
    while (true)
    {
        toSend.emplace("button", "broadcaster", Pulse::Low);
        while (!toSend.empty())
        {
            Message m = toSend.front();
            toSend.pop();
            if (m.p == Pulse::Low && m.receiver == "rx")
            {
                std::cout << i << '\n';
                return;
            }
            if (modules.find(m.receiver) == modules.end())
            {
                continue;
            }
            auto nextMessages = modules.at(m.receiver).HandlePulse(m);
            for (const auto &nextMessage : nextMessages)
            {
                toSend.push(nextMessage);
            }
        }
        i++;
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