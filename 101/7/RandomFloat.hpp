#pragma once
#include <time.h>
#include <random>
#include <queue>

class RandomFloat
{
public:
    std::queue<float> qu;
    bool initialized = false;
    RandomFloat()
    {
        qu = std::queue<float>();
        fill();
    }

    ~RandomFloat()
    {
    }

    void fill()
    {
        srand((unsigned)time(NULL));
        for (int i = 0; i < 1000; i++)
        {
            qu.push((float)rand() / (RAND_MAX));
        }
    }

    float get()
    {
        float result = qu.front();
        qu.pop();
        if (qu.empty())
        {
            fill();
        }
        return result;
    }
};
