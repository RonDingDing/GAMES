//
// Created by goksu on 2/25/20.
//
#include "Scene.hpp"
#include "Data.hpp"

#pragma once
struct hit_payload
{
    float tNear;
    uint32_t index;
    Vector2f uv;
    Object *hit_obj;
};

class Renderer
{
public:
    // void Render(const Scene& scene);
    void Render(const Scene &scene, int spp = 16, int max_recursion = 10, float russian_roulette = 0.8f);
    void RayTask(Data &data);

private:
    MyThreadPool<float, std::function<float(Data)>, Data> pool;
};
