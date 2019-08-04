cmake_minimum_required(VERSION 3.10.0)
project(gecko)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
# cmake_host_system_information(RESULT CORES QUERY NUMBER_OF_LOGICAL_CORES)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")

option(GECKO_VERBOSE "Print build debug info" OFF)
if(GECKO_VERBOSE)
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(CMAKE_RULE_MESSAGES OFF)
endif()

# set path for FindXXX.cmake
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND ALL_LIBS "")

# Add local -----------------------------------------------------
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

if (NOT APPLE)
#     set(msgpack_DIR $ENV{HOME}/.local/lib/cmake/msgpack)
#     include_directories($ENV{HOME}/.local/include)
#     link_directories($ENV{HOME}/.local/lib)
else()
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Json ------------------------------------------------------------
# include_directories(extlibs/nlohmann-json/include)

# Threads ---------------------------------------------------------
include(cmake/threads.cmake)

# Marko -----------------------------------------------------------
# include_directories(extlibs/marko/lib/include)
# link_directories(extlibs/marko/build)
# list(APPEND ALL_LIBS "marko")
add_subdirectory(extlibs/marko)

# ZeroMQ ----------------------------------------------------------
include(cmake/zmq.cmake)

# git submodule add https://github.com/zeromq/cppzmq.git
# turn this on with cmake -DBUILD_CXXOPTS=ON
# option(BUILD_CXXOPTS "Build command line options cxxopts" OFF)
# if(BUILD_CXXOPTS)
#     message(STATUS ">> Compiling cxxopts")
#     include_directories(gecko-msgpack)
# endif(BUILD_CXXOPTS)


# Build Gecko library ----------------------------------------------
file(GLOB SOURCES src/*.cpp)
include_directories(src)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${ALL_LIBS}
        marko
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        # extlibs/marko/lib/include
        extlibs/nlohmann-json/include
        extlibs/cppzmq
)

list(APPEND ALL_LIBS ${PROJECT_NAME})

# Build Gecko-MsgPack ----------------------------------------------
# git submodule add git@github.com:gecko-robotics/gecko-msgpack.git
# turn this on with cmake -DBUILD_MSGPACK=ON
option(BUILD_MSGPACK "Build the message pack messages" ON)
if(BUILD_MSGPACK)
    include(cmake/msgpack.cmake)
    add_subdirectory(extlibs/gecko-msgpack)
endif(BUILD_MSGPACK)


# Build tools -------------------------------------------------------
add_subdirectory(tools)

# Tests --------------------------------------------------------------
# enable_testing()
add_subdirectory(gtests)

# Examples -----------------------------------------------------------
add_subdirectory(example/core)
add_subdirectory(example/json)
add_subdirectory(example/node)
# add_subdirectory(example/opencv)
add_subdirectory(example/simple)

# Package Generator --------------------------------------------------
include(cmake/package.cmake)

#=====================================================================
if(1)
    # Local Install for Testing/Dev --------------------------------------
    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)

    file(GLOB HEADERS src/gecko/*.hpp)
    install(FILES ${HEADERS} DESTINATION include/gecko)

    # CppZmq -----------------------------------------------------------
    install(
        FILES extlibs/cppzmq/zmq.hpp extlibs/cppzmq/zmq_addon.hpp
        DESTINATION include/gecko/zmq
    )

    # MsgPack-Gecko -----------------------------------------------------
    if(BUILD_MSGPACK)
        file(GLOB HEADERS extlibs/gecko-msgpack/gecko/msgpack/*.hpp)
        install(FILES ${HEADERS} DESTINATION include/gecko/msgpack)
    endif()

    # Json ---------------------------------------------------------------
    # file(GLOB_RECURSE HEADERS extlibs/nlohmann-json/include/*)
    # install(FILES ${HEADERS} DESTINATION include/gecko/json)
    install(
        DIRECTORY extlibs/nlohmann-json/include/
        DESTINATION include/gecko/json
    )

    # Marko ---------------------------------------------------------------
    # install(
    #     DIRECTORY extlibs/marko/lib/include/
    #     DESTINATION include
    # )
    # install(
    #     TARGETS extlibs/marko/marko
    #     DESTINATION lib
    # )
endif()
