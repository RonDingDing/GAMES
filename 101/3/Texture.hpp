//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture
{
private:
    cv::Mat image_data;

public:
    Texture(const std::string &name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        u = std::min({1.0f, std::max({0.0f, u})});
        v = std::min({1.0f, std::max({0.0f, v})});

        auto u_img = u * width;
        auto v_img = (1 - v) * height;

        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        u = std::min({1.0f, std::max({0.0f, u})});
        v = std::min({1.0f, std::max({0.0f, v})});

        float u_img = u * width;
        float v_img = (1 - v) * height;

        float u_left, u_right, v_up, v_down;

        int x_floor = std::floor(u_img);
        int y_floor = std::floor(v_img);

        // u_left = x_floor;
        // u_right = x_floor + 1;
        // v_up = y_floor;
        // v_down = y_floor + 1;

        if (u_img - x_floor < 0.5)
        {
            u_left = x_floor - 0.5;
            u_right = x_floor + 0.5;
        }
        else
        {
            u_left = x_floor + 0.5;
            u_right = x_floor + 1.5;
        }

        if (v_img - y_floor < 0.5)
        {
            v_up = y_floor - 0.5;
            v_down = y_floor + 0.5;
        }
        else
        {
            v_up = y_floor + 0.5;
            v_down = y_floor + 1.5;
        }

        if (v_up > height)
        {
            v_up = height;
        }
        if (u_right > width)
        {
            u_right = width;
        }

        auto u00_color = image_data.at<cv::Vec3b>(v_up, u_left);
        auto u01_color = image_data.at<cv::Vec3b>(v_down, u_left);
        auto u10_color = image_data.at<cv::Vec3b>(v_up, u_right);
        auto u11_color = image_data.at<cv::Vec3b>(v_down, u_right);

        cv::Vec3b color_x_00_01 = u00_color + (u_img - u_left) * (u01_color - u00_color);
        cv::Vec3b color_x_10_11 = u01_color + (u_img - u_left) * (u11_color - u10_color);
        cv::Vec3b color = color_x_00_01 + (v_img - v_up) * (color_x_10_11 - color_x_00_01);
        cv::Vec3b color_o = image_data.at<cv::Vec3b>(v_img, u_img);

        Eigen::Vector3f c_00 = Eigen::Vector3f(u00_color[0], u00_color[1], u00_color[2]);
        Eigen::Vector3f c_01 = Eigen::Vector3f(u01_color[0], u01_color[1], u01_color[2]);
        Eigen::Vector3f c_10 = Eigen::Vector3f(u10_color[0], u10_color[1], u10_color[2]);
        Eigen::Vector3f c_11 = Eigen::Vector3f(u11_color[0], u11_color[1], u11_color[2]);

        Eigen::Vector3f color_oo = Eigen::Vector3f(color_o[0], color_o[1], color_o[2]);
        Eigen::Vector3f color_1 = Eigen::Vector3f(color[0], color[1], color[2]);
        // std::cout << "(" << color_oo[0] << ", " << color_oo[1] << ", " << color_oo[2] << ") (" << color_1[0] << ", " << color_1[1] << ", " << color_1[2] << ")" << std::endl;

        // std::cout << "(" << color_oo[0] << ", " << color_oo[1] << ", " << color_oo[2] << ") (" << c_00[0] << ", " << c_00[1] << ", " << c_00[2] << ") (" << c_01[0] << ", " << c_01[1] << ", " << c_01[2] << ") (" << c_10[0] << ", " << c_10[1] << ", " << c_10[2] << ") (" << c_11[0] << ", " << c_11[1] << ", " << c_11[2] << ")" <<  std::endl;

        // std::cout <<  u_left  << ", " <<  u_img  << ", " <<  u_right  << " | " <<   v_up  << ", " <<  v_img  << ", " <<  v_down  << std::endl;
        // std::cout <<  u_left  << ", " <<  u_img  << ", " <<  u_right  << " | " <<   v_up  << ", " <<  v_img  << ", " <<  v_down  << std::endl;
        return color_1;
    }
};
#endif //RASTERIZER_TEXTURE_H
