cmake_minimum_required(VERSION 3.10)
PROJECT("statemachine")
#add_definitions(-std=c++11) # Use C++11
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

# Add local -----------------------------------------------------
# include_directories(./include)
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)


# Build Gecko library ----------------------------------------------
add_executable(${PROJECT_NAME} sm.cpp)
