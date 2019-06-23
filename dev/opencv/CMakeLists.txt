cmake_minimum_required(VERSION 3.10)
project(json)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# find_package(OpenCV REQUIRED)
include(../../cmake/opencv.cmake)

add_executable(main opencv-test.cpp)
target_link_libraries(main ${OpenCV_LIBS})
