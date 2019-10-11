cmake_minimum_required(VERSION 3.10)
project(cxxopts)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(./cxxopts-master/include)

file(GLOB SOURCES *.cpp)

message(STATUS "Gecko Tests ----------------------")
foreach(src ${SOURCES})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    # target_link_libraries(${name} cxxopts)
endforeach()
