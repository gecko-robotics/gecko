cmake_minimum_required(VERSION 3.10)
project(json)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND ALL_LIBS "")

# Load in pkg-config support ------------------------------------
find_package(PkgConfig)

# CMake ---------------------------------------------------------
include(../../cmake/msgpack.cmake)
include(../../cmake/threads.cmake)
include(../../cmake/zmq.cmake)
include(../../cmake/opencv.cmake)

add_executable(main opencv-test.cpp)
target_link_libraries(main ${OpenCV_LIBS})

add_executable(camera-test camera-test.cpp)
target_link_libraries(camera-test ${OpenCV_LIBS} ${ALL_LIBS})
