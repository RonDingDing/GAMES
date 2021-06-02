cmake_minimum_required(VERSION 3.10)
project(BezierCurve)
set(OpenCV_DIR "D:/GAMESlibraries/opencv/build/x64/minggw")  # 设置的文件夹一定要是minggw的build
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 14)
add_executable(BezierCurve main.cpp)
target_link_libraries(BezierCurve ${OpenCV_LIBRARIES})






