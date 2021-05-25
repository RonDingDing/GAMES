#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 
                 0, 1, 0, -eye_pos[1], 
                 0, 0, 1, -eye_pos[2], 
                 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model;

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float alpha = rotation_angle / 180.0f * M_PI;
    Eigen::Matrix4f rotation_z;
    rotation_z << cos(alpha), -sin(alpha), 0, 0,
                  sin(alpha), cos(alpha),  0, 0,
                  0,          0,           1, 0,
                  0,          0,           0, 1;
    model = rotation_z;
    return model;
}

Eigen::Matrix4f get_rotation_matrix(Vector3f axis, float angle)
{
    Eigen::Matrix4f rotation;
    Eigen::Matrix3f N, R;                            //R是罗德里格斯矩阵
    Eigen::Matrix3f I = Eigen::Matrix3f::Identity(); //I是单位矩阵
    Eigen::Vector3f norm = axis.normalized();
    float x = norm[0];
    float y = norm[1];
    float z = norm[2];
    
    double rotate = angle / 180.0f * M_PI;
    N << 0, -z, y,
         z, 0,  -x,
        -y, x,  0;
    if (0 == x && x == y && y == z) 
    {
        R = I;
    } 
    else 
    {
        R = cos(rotate) * I + (1 - cos(rotate)) * norm * norm.transpose() + sin(rotate) * N;
    }
   
    rotation << R(0, 0), R(0, 1), R(0, 2), 0,
                R(1, 0), R(1, 1), R(1, 2), 0,
                R(2, 0), R(2, 1), R(2, 2), 0,
                0,       0,       0,       1;
    return rotation;
}



Eigen::Matrix4f get_rotation_matrix_x_y_z(Eigen::Vector3f axis, float angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f rotate;
    float alpha = angle / 180.0f * M_PI;
    //绕X轴
    Eigen::Vector3f norm = axis.normalized();
    if (norm[0] != 0)
    {
        rotate << 1, 0,             0,           0,
                  0, cos(alpha),    -sin(alpha), 0,
                  0, sin(alpha),    cos(alpha),  0,
                  0, 0,             0,           1;
        model = model * rotate;
    }
    //绕Y轴
    if (norm[1] != 0)
    {
        rotate << cos(alpha),  0, sin(alpha), 0,
                  0,           1, 0,          0,
                  -sin(alpha), 0, cos(alpha), 0,
                  0,           0, 0,          1;

        model = model * rotate;
    }
    //绕Z轴
    if (norm[2] != 0)
    {
        rotate << cos(alpha), -sin(alpha), 0, 0,
                  sin(alpha), cos(alpha),  0, 0,
                  0,          0,           1, 0,
                  0,          0,           0, 1;
        model = model * rotate;
    }
    return model;
}
 


Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection;

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    Eigen::Matrix4f orth, persp_to_orth;
    float half_eye_angle = eye_fov / 2 / 180.0 * MY_PI;
    float n = -zNear;
    float f = -zFar;
    float t = zNear * tan(half_eye_angle);
    float r = t * aspect_ratio;
    float l = -r;
    float b = -t;

    persp_to_orth << n, 0, 0,     0,
                     0, n, 0,     0,
                     0, 0, n + f, -n * f,
                     0, 0, 1,     0;

    orth << 2 / (r - l), 0,           0,         0,
            0,           2 / (t - b), 0,         0,
            0,           0,           2 / (n-f), 0,
            0,           0,           0,         1;

    
    projection = orth * persp_to_orth;
    return projection;

}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";
    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc >= 4) {
            filename = std::string(argv[3]);
        }
    }
    rst::rasterizer r(700, 700); 

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;
    if (command_line == true) {
        
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }
    float x = 0, y = 0, z = 0;
    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // r.set_model(get_model_matrix(angle)); // 基础题
        Eigen::Vector3f forv(x, y, z); // 提高题（1.罗德里格公式解法、2.欧拉角解法）
        r.set_model(get_rotation_matrix(forv, angle)); // 提高题（1.罗德里格公式解法）
        // r.set_model(get_rotation_matrix_x_y_z(forv, angle)); // 提高题（2.欧拉角解法）
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);
        Eigen::Vector3f norm = forv.normalized().transpose();
        if (key == 'r') {
            angle += 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        else if (key == 'f') {
            angle -= 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        } 
        else if (key == 'q') {
            z += 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        else if (key == 'e') {
            z -= 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        else if (key == 'a') {
            y += 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        else if (key == 'd') {
            y -= 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        } 
        else if (key == 'w') {
            x += 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        else if (key == 's') {
            x -= 1;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        else if (key == 'g') {
            x = y = z = angle = 0;
            std::cout << "x, y, z, angle: " << x << ", " << y << ", " << z << ", " << angle << std::endl;
        }
        
    }

    return 0;
}

