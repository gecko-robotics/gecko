cmake_minimum_required(VERSION 3.10.0)
project(gecko-msgpack)

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

find_package(PkgConfig)
include(../../../cmake/msgpack.cmake)
include(../../../cmake/zmq.cmake)

list(APPEND msgpktests
    # msgpack-test.cpp
    # # msgpack-2-test.cpp
    # # msgpack-3-test.cpp
    # msgpack-4-test.cpp
    # msgpack-5-test.cpp
    # msgpack-6-test.cpp
    # msgpack-7-test.cpp
    # msgpack-8-test.cpp
    # # msgpack-9-test.cpp
    # msgpack-10-test.cpp
    # # msgpack-zlib-test.cpp
    # # zlib-test.cpp
    # # multicast-test.cpp
    # # multicast-2-test.cpp
    bag-test.cpp
)

set(MSGPACK_ALL
    ${MSGPACK_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ZMQ_LIBRARIES}
)

message(STATUS "MsgPack Dev ----------------------")
foreach(src ${msgpktests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name}
        PUBLIC
            ${MSGPACK_ALL}
            ${ALL_LIBS}
    )
    target_include_directories(${name}
        PUBLIC
            # extlibs/marko/lib/include
            ../../../extlibs/nlohmann-json/include
            ../../../extlibs/cppzmq
    )
    target_link_directories(${name}
        PUBLIC
            ../../../build
            /usr/local/lib
    )
endforeach()
