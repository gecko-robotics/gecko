cmake_minimum_required(VERSION 3.10)
project(json)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(./cxxopts-master/include)

add_executable(example example.cpp)
# target_link_libraries(example cxxopts)
