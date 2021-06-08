#pragma once

#include <iostream>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
#include <map>
#include <atomic>

template <typename ReturnType, typename FunctionType, typename... ArgsType>
class MyThreadPool
{
public:
    MyThreadPool()
    {
        std::mutex mutexing;
        std::map<int, ReturnType> mapping{};
    };

    void add_task(int id, FunctionType func, ArgsType... params)
    {
        std::unique_lock<std::mutex> lock(mutexing);
        mapping[id] = std::async(std::launch::deferred, func, params...);
    };

    ReturnType get(int id)
    {
        std::unique_lock<std::mutex> lock(mutexing);
        auto iter = mapping.find(id);
        if (iter != mapping.end())
        {
            auto result = iter->second.get();
            return result;
        }

        throw std::out_of_range("Not in dict");
    }

private:
    std::mutex mutexing;
    std::map<int, std::future<ReturnType>> mapping;
};
