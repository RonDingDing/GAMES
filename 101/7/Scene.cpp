//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"
void Scene::buildBVH()
{
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum)
            {
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
    const Ray &ray,
    const std::vector<Object *> &objects,
    float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear)
        {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }

    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth, int max_recursion, float russian_roulette) const
{
    Vector3f zero(0);
    Vector3f lighting_direct = zero;
    Vector3f lighting_indirect = zero;

    Intersection object_inter = Scene::intersect(ray);
    if ((!object_inter.happened) || (depth > max_recursion))
    {
        return zero;
    }
    //打到光源
    if (object_inter.m->hasEmission())
    {
        return object_inter.m->getEmission();
    }
    //打到物体后对光源均匀采样
    Intersection light_inter;
    float pdf_light = 0.0f;
    sampleLight(light_inter, pdf_light); //获得对光源的采样，包括光源的位置和采样的pdf
    Vector3f p = object_inter.coords;
    Vector3f x = light_inter.coords;
    Vector3f x_minus_p = x - p;
    Vector3f N = object_inter.normal;
    Vector3f NN = light_inter.normal;
    float dis = x_minus_p.x * x_minus_p.x + x_minus_p.y * x_minus_p.y + x_minus_p.z * x_minus_p.z;
    Vector3f ws = x_minus_p.normalized();
    Vector3f wo = ray.direction;

    Ray reversed_ray(p, ws);

    Intersection reversed_ray_inter = Scene::intersect(reversed_ray);
    //lighting_direct = L_i * f_r * cos_theta * cos_theta_x / |x - p | ^ 2 / pdf_light
    if (reversed_ray_inter.distance - x_minus_p.norm() > -EPSILON) //说明之间没有遮挡
    {
        Vector3f emission = light_inter.emit;
        Vector3f f_r = object_inter.m->eval(wo, ws, N);
        lighting_direct = emission * f_r * dotProduct(ws, N) * dotProduct(-ws, NN) / dis / pdf_light;

    } // 打到物体后对半圆随机采样使用RR算法
    if (get_random_float() < russian_roulette)
    {

        Vector3f wi = object_inter.m->sample(wo, N).normalized();
        Ray roulette_ray(p, wi);
        Intersection islight = Scene::intersect(roulette_ray);
        // shade(q, wi) * f_r * cos_theta / pdf_hemi / P_RR
        if (islight.happened && !islight.m->hasEmission())
        {
            float pdf_wo_wi_N = object_inter.m->pdf(wo, wi, N);
            Vector3f recursive_cast = castRay(roulette_ray, depth + 1, max_recursion, russian_roulette);
            Vector3f f_r2 = object_inter.m->eval(wo, wi, N);
            lighting_indirect = recursive_cast * f_r2 * dotProduct(wi, N) / pdf_wo_wi_N / russian_roulette;
        }
    }
    return lighting_direct + lighting_indirect;
}