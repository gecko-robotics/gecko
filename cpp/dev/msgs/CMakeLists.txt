cmake_minimum_required(VERSION 3.10)
PROJECT("msg-test")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

list(APPEND ALL_LIBS "")

# Load in pkg-config support ------------------------------------
find_package(PkgConfig)

# CMake ---------------------------------------------------------
include(../../cmake/msgpack.cmake)
include(../../cmake/threads.cmake)
include(../../cmake/zmq.cmake)

# Build test ---------------------------------------------------
add_executable(msg-test msg-test.cpp)
target_link_libraries(msg-test ${ALL_LIBS})


add_executable(common-msg-test common-msg-test.cpp)
target_link_libraries(common-msg-test ${ALL_LIBS})
