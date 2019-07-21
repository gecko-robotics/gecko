cmake_minimum_required(VERSION 3.10.0)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "-------------------------------------")

# set path for FindXXX.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

list(APPEND ALL_LIBS "")


# Get msgpack -----------------------------------------------------
include_directories(
    /usr/local/include
    ../../src
    ../../extlibs/cppzmq
    ../../extlibs/gecko-msgpack)
link_directories(/usr/local/lib ../../build)

find_package(PkgConfig)
include(../../cmake/msgpack.cmake)

# find_package(ZLIB)
# if(ZLIB_FOUND)
#     message(STATUS ">> zlib Found: ${ZLIB_VERSION}")
#     list(APPEND ALL_LIBS ${ALL_LIBS} ${ZLIB_LIBRARIES})
# else()
#     message(WARNING "\n<<--- Could not find zlib --->>\n")
# endif()

# Threads ---------------------------------------------------------
include(../../cmake/threads.cmake)

# ZeroMQ ----------------------------------------------------------
include(../../cmake/zmq.cmake)

# Build Project ----------------------------------------------
file(GLOB SOURCES *.cpp)

list(APPEND ALL_LIBS ${ALL_LIBS} gecko)

foreach(src ${SOURCES})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${ALL_LIBS})
endforeach()
