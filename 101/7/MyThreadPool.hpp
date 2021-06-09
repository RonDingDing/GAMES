#pragma once
#include <functional>
#include <future>
#include <iostream>

template <typename FunctionType>
class AsyncFunctor
{
    FunctionType func;

public:
    AsyncFunctor(FunctionType &&func) : func(func) {}

    template <typename... ArgsTypes>
    std::future<typename std::result_of<FunctionType(ArgsTypes...)>::type>
    operator()(ArgsTypes &&...args) const
    {
        return std::async(std::launch::deferred, func, args...);
    }
};

template <typename FunctionType> // helper function to handle template parameters
AsyncFunctor<FunctionType> AsyncFunction(FunctionType &&func)
{
    return AsyncFunctor<FunctionType>(std::move(func));
}

