#include <iostream>
#include <set>
#include <chrono>

#include "RbTree.hpp"

int main()
{
    std::set<int> stdSet{};
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 30'000; ++i)
    {
        stdSet.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = end - start;
    std::cout << "Time for inserting 30'000 elements in std::set [" 
              << dur.count() << " nanosec]" << std::endl;

    ads::RbTree<int> adsSet;
    start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 30'000; ++i)
    {
        adsSet.Insert(i);
    }

    end = std::chrono::high_resolution_clock::now();
    dur = end - start;
    std::cout << "Time for inserting 30'000 elements in ads::RbTree [" 
              << dur.count() << " nanosec]" << std::endl;

}