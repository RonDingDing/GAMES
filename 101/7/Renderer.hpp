//
// Created by goksu on 2/25/20.
//
#include "Scene.hpp"
#include "Data.hpp"
#include <random>

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
    Renderer() { srand((unsigned)time(NULL)); };
    void Render(const Scene &scene, int spp = 16, int max_recursion = 10, float russian_roulette = 0.8f);
    Vector3f RayTask(Data *data);
    Vector3f RayTaskAsync(Data *data);
    Vector3f RayTaskDefer(Data *data);
    Vector3f RayTaskPool(Data *data);

private:
};
