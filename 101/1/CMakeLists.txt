cmake_minimum_required(VERSION 3.10)
project(Rasterizer)
set(OpenCV_DIR "C:/Program Files (x86)/opencv/build/x64/minggw")  # 设置的文件夹一定要是minggw的build
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
include_directories("C:/Program Files (x86)/Eigen3/include")  # include Eigen3 所需的文件

add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp Triangle.hpp Triangle.cpp)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
