// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <cmath>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


static bool insideTriangle(int x, int y, const Eigen::Vector3f* _v)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Eigen::Vector3f A = _v[0];
    Eigen::Vector3f B = _v[1];
    Eigen::Vector3f C = _v[2];

    Eigen::Vector2f AB(B.x() - A.x(), B.y() - A.y());
    Eigen::Vector2f BC(C.x() - B.x(), C.y() - B.y());
    Eigen::Vector2f CA(A.x() - C.x(), A.y() - C.y());

    Eigen::Vector2f AQ(x - A.x(), y - A.y());
    Eigen::Vector2f BQ(x - B.x(), y - B.y());
    Eigen::Vector2f CQ(x - C.x(), y - C.y());

    float c1 = AB[0] * AQ[1] - AB[1] * AQ[0];  
	float c2 = BC[0] * BQ[1] - BC[1] * BQ[0];
	float c3 = CA[0] * CQ[1] - CA[1] * CQ[0];
    if ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0))
    {
        return true;
    }
    return false;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw_msaa(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type, int msaa_n)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
            // vert.z() = -vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle_msaa(t, msaa_n);
    }
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            // vert.z() = vert.z() * f1 + f2;
            vert.z() = -vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }
}

void rst::rasterizer::rasterize_triangle_msaa(const Triangle& t, int msaa_n) 
{
    //Screen space rasterization 基础题
    auto v = t.toVector4();
    int min_x = std::min({std::floor(v[0].x()),std::floor(v[1].x()), std::floor(v[2].x())});
    int max_x = std::max({std::ceil(v[0].x()),std::ceil(v[1].x()), std::ceil(v[2].x())});
    int min_y = std::min({std::floor(v[0].y()),std::floor(v[1].y()), std::floor(v[2].y())});
    int max_y = std::max({std::ceil(v[0].y()),std::ceil(v[1].y()), std::ceil(v[2].y())});

    for (int x = min_x; x < max_x; x ++)
    {
        for (int y = min_y; y < max_y; y ++)
        {
            int count = 0;
            float min_depth = FLT_MAX;
            float n = 1 / static_cast<float>(msaa_n);
            msaa_n = std::max({1, msaa_n});
            for (float i = 0; i < msaa_n; i +=1)
            {
                for (float j = 0; j < msaa_n; j +=1)
                {
                    float x_small_pixel = x + 1 / (n * 2) + i * 1 / n; 
                    float y_small_pixel = y + 1 / (n * 2) + j * 1 / n; 
                    
                    if (insideTriangle(x_small_pixel, y_small_pixel, t.v))
                    {
                        auto[alpha, beta, gamma] = computeBarycentric2D(x_small_pixel, y_small_pixel, t.v);
                        float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                        z_interpolated *= w_reciprocal;
                        min_depth = std::min({min_depth, z_interpolated});
                        count ++;
                    }
                }
            }

            if (count > 0)
            {
                int index = get_index(x, y);
                if (min_depth < depth_buf[index])
                {
                    depth_buf[index] = min_depth;
                    Eigen::Vector3f color = t.getColor() * count / (msaa_n * msaa_n);
                    Eigen::Vector3f pixel(x, y, 1.0f);
                    set_pixel(pixel, color);
                }
            }
        }        
    }
}

void rst::rasterizer::rasterize_triangle(const Triangle& t) 
{
    //Screen space rasterization 基础题
    auto v = t.toVector4();
    int min_x = std::min({std::floor(v[0].x()),std::floor(v[1].x()), std::floor(v[2].x())});
    int max_x = std::max({std::ceil(v[0].x()),std::ceil(v[1].x()), std::ceil(v[2].x())});
    int min_y = std::min({std::floor(v[0].y()),std::floor(v[1].y()), std::floor(v[2].y())});
    int max_y = std::max({std::ceil(v[0].y()),std::ceil(v[1].y()), std::ceil(v[2].y())});

    for (int x = min_x; x < max_x; x ++)
    {
        for (int y = min_y; y < max_y; y ++)
        {
            if (insideTriangle(x, y, t.v))
            {
                auto[alpha, beta, gamma] = computeBarycentric2D(x + 0.5, y + 0.5, t.v);
                float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                z_interpolated *= w_reciprocal;

                int index = get_index(x, y);
                if (z_interpolated < depth_buf[index])
                {
                    depth_buf[index] =  z_interpolated;
                    Eigen::Vector3f color = t.getColor();
                    Eigen::Vector3f pixel(x, y, 1.0f);
                    set_pixel(pixel, color);
                }
            }
        }        
    }

    // TODO : Find out the bounding box of current triangle.
    // iterate through the pixel and find if the current pixel is inside the triangle

    // If so, use the following code to get the interpolated z value.
    //auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
    //float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    //float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    //z_interpolated *= w_reciprocal;

    // TODO : set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format off