# cmake_minimum_required(VERSION 3.6)
# project(marko)
#
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
#
# message(STATUS "-------------------------------------")
# message(STATUS "  Project: ${PROJECT_NAME}")
# message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
# message(STATUS "-------------------------------------")

# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

# file(GLOB SOURCES lib/*.cpp)

# add_library(marko SHARED ${SOURCES})

# file(GLOB SOURCES ../lib/*.cpp)

# add_library(marko SHARED ${SOURCES})
# target_include_directories(marko
#     PUBLIC
#         ../lib/include
# )

# include_directories(../lib/include)
# link_directories(build)

add_executable(request request.cpp)
target_link_libraries(request marko)

add_executable(reply reply.cpp)
target_link_libraries(reply marko)

add_executable(listen listen.cpp)
target_link_libraries(listen marko)
