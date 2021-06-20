CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(TinyRasterizer)

set(CMAKE_CXX_STANDARD 17)


SET(CMAKE_BUILD_TYPE "Debug") 

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

ADD_EXECUTABLE(TinyRasterizer main.cpp VectorN.hpp Buffers.hpp Triangle.hpp PpmImage.hpp Mesh.hpp ObjLoader.hpp TgaImage.hpp)