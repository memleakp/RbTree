#include <iostream>
#include <set>
#include <string>
#include <chrono>
#include <array>

#include "RbTree.hpp"

/// Class `Timer` is used for measuring performance of a code block.
/// It fix a `time_point` of instantiation and destruction and prints
/// execution time of a code block in milliseconds.
class Timer
{
public:
    Timer(const std::string& messagePrefix)
        : m_start{ std::chrono::high_resolution_clock::now() }
        , m_messagePrefix{ messagePrefix }
    {}

    ~Timer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto executionTime = end - m_start;
        std::cout << m_messagePrefix << "Execution time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(executionTime).count()
            << " millisec" << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point m_start;
    std::string m_messagePrefix;
};

static void CheckRbTreePrinting()
{
    // values to insert
    std::array<int, 10> values{ 10, 12, 5, 7, 0, 14, 20, 8, 9, 1 };
    ads::RbTree<int> set{};

    std::size_t idx = 0;
    for (const auto& entry : values)
    {
        set.Insert(entry);
        std::cout << "\nIdx " << (idx++) << ":" << std::endl;
        set.Dump();
    }
}

int main()
{
    {
        std::set<int> stdSet{};
        ads::RbTree<int> adsSet{};

        {
            Timer _{ "Insertion in std::set    " };
            for (int i = 0; i < 30'000; ++i)
                stdSet.insert(i);
        }
        {
            Timer _{ "Insertion in ads::RbTree " };
            for (int i = 0; i < 30'000; ++i)
                adsSet.Insert(i);
        }
        {
            Timer _{ "Searching in std::set    " };
            for (int i = 0; i < 30'000; ++i)
                stdSet.find(i);
        }
        {
            Timer _{ "Searching in ads::RbTree " };
            for (int i = 0; i < 30'000; ++i)
                adsSet.Find(i);
        }
    }
    {
        CheckRbTreePrinting();
    }
}
