#pragma once
#include "Scene.hpp"
#include "MyThreadPool.hpp"

class Data
{

public:
    Scene *scene;
    std::vector<Vector3f> &framebuffer;
    Vector3f eye_pos;
    Vector3f dir;
    int max_recursion;
    float russian_roulette;
    int spp;
    int m;

    Data(Scene *scene, std::vector<Vector3f> &framebuffer, Vector3f eye_pos, Vector3f dir, int max_recursion, float russian_roulette, int spp, int m)
        : scene(scene), framebuffer(framebuffer), eye_pos(eye_pos), dir(dir), max_recursion(max_recursion), russian_roulette(russian_roulette), spp(spp), m(m) {}
};