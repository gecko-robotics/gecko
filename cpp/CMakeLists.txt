cmake_minimum_required(VERSION 3.10.0)
project(gecko VERSION 0.5.1 LANGUAGES CXX)

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

# gecko.cmake ----------------------------------------------------
# set(WS "${PROJECT_SOURCE_DIR}/gecko_workspace.txt")
set(GECKO_VERSION "gecko")
set(GECKO_BASE_PATH "/opt/gecko/${GECKO_VERSION}")
# IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# else()
message(STATUS ">> Setting install base path: ${GECKO_BASE_PATH}")
set(CMAKE_INSTALL_PREFIX ${GECKO_BASE_PATH})
# ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(GECKO_BIN     "${GECKO_BASE_PATH}/bin")
set(GECKO_LIB     "${GECKO_BASE_PATH}/lib")
set(GECKO_CMAKE   "${GECKO_BASE_PATH}/lib/cmake")
set(GECKO_INCLUDE "${GECKO_BASE_PATH}/include/gecko")

# ----------------------------------------------------------------

# set path for FindXXX.cmake
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND ALL_LIBS "")

include_directories(/opt/gecko/include)
link_directories(/opt/gecko/lib)
link_directories(/usr/local/lib)

set(CMAKE_PREFIX_PATH "/opt/gecko/lib/cmake;/opt/gecko/share/cmake")

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Json ------------------------------------------------------------
# include_directories(extlibs/nlohmann-json/include)
find_package(nlohmann_json REQUIRED)
if(nlohmann_json_FOUND)
    message(STATUS ">> json found: ${nlohmann_json_VERSION}")
endif()

# Threads ---------------------------------------------------------
include(cmake/threads.cmake)

# Marko -----------------------------------------------------------
# include_directories(extlibs/marko/lib/include)
# link_directories(extlibs/marko/build)
# list(APPEND ALL_LIBS "marko")
add_subdirectory(extlibs/marko)

# ZeroMQ ----------------------------------------------------------
include(cmake/zmq.cmake)
find_package(cppzmq REQUIRED)
if(cppzmq_FOUND)
    message(STATUS ">> cppZMQ found: ${cppzmq_VERSION}")
endif()

# Build Gecko library ----------------------------------------------
message(STATUS ">> libs found: ${ALL_LIBS}")

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
        # extlibs/nlohmann-json/include
        # /opt/gecko/include/cppzmq
        nlohmann_json::nlohmann_json
        cppzmq
)

list(APPEND ALL_LIBS ${PROJECT_NAME})
# add_subdirectory(extlibs/gecko-msgpack)

# Build Gecko-MsgPack ----------------------------------------------
# git submodule add git@github.com:gecko-robotics/gecko-msgpack.git
# turn this on with cmake -DBUILD_MSGPACK=ON
# option(BUILD_MSGPACK "Build the message pack messages" ON)
# if(BUILD_MSGPACK)
#     include(cmake/msgpack.cmake)
#     add_subdirectory(extlibs/gecko-msgpack)
# endif(BUILD_MSGPACK)


# Build tools -------------------------------------------------------
add_subdirectory(tools)

# Gecko MsgPack ------------------------------------------------------
# add_subdirectory(extlibs/gecko-msgpack)
# include_directories(extlibs/gecko-msgpack)


# Protobuf ------------------------------------------------------
find_package(Protobuf REQUIRED)
if ( Protobuf_FOUND )
    # include_directories(${Protobuf_INCLUDE_DIRS})
    # include_directories(${CMAKE_CURRENT_BINARY_DIR})
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
    message( STATUS "Protobuf compiler libraries : ${Protobuf_PROTOC_LIBRARIES}")
    message( STATUS "Protobuf lite libraries : ${Protobuf_LITE_LIBRARIES}")

    set(BASE_MSG "./extlibs/protobuf")
    set(MSG_PROTOS
        # ${BASE_MSG}/standard_msgs.proto
        # ${BASE_MSG}/nav_msgs.proto
        # ${BASE_MSG}/sensor_msgs.proto
        ${BASE_MSG}/msgs.proto
    )

    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${MSG_PROTOS})

    message(STATUS "[>>] ${PROTO_SRCS} \n${PROTO_HDRS} \n${BASE_MSG}/${MSG_PROTOS}")

    # add_library(proto SHARED ${PROTO_SRCS} ${PROTO_HDRS})
    # file(GLOB PROTO_SRCS build/proto/*.pb.cc)
    # file(GLOB PROTO_HDRS build/proto/*.pb.h)
    add_library(proto
        SHARED
            ${PROTO_SRCS}
            ${PROTO_HDRS}
    )
    target_include_directories(proto
        PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR}
            ${Protobuf_INCLUDE_DIRS}
    )
    target_link_libraries(
        proto
        ${Protobuf_LIBRARIES}
    )

    list(APPEND ALL_LIBS proto)

else()
    message( WARNING "Protobuf package not found -> set PROTOBUF_ROOT variable")
endif()


# Tests --------------------------------------------------------------
# enable_testing()
add_subdirectory(gtests)

# Examples -----------------------------------------------------------
add_subdirectory(examples)
# add_subdirectory(example/json)
# add_subdirectory(example/node)
# add_subdirectory(example/opencv)
# add_subdirectory(example/simple)

# Package Generator --------------------------------------------------
# include(cmake/package.cmake)

#=====================================================================
if(1)
    # this doesn't seem to execute in order
    # install(CODE "execute_process(COMMAND ${PROJECT_SOURCE_DIR}/install_setup.sh ${GECKO_BASE_PATH})")

    # Local Install for Testing/Dev --------------------------------------
    # set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
    # install(TARGETS ${PROJECT_NAME} DESTINATION lib)
    install(TARGETS ${PROJECT_NAME}
      ARCHIVE DESTINATION ${GECKO_LIB}  # static libraries
      LIBRARY DESTINATION ${GECKO_LIB}  # shared libraries
      RUNTIME DESTINATION ${GECKO_BIN}  # executables
    )

    install(DIRECTORY src/${PROJECT_NAME}/
      DESTINATION ${GECKO_INCLUDE}
      FILES_MATCHING PATTERN "*.hpp"
      PATTERN ".git" EXCLUDE
    )

    # file(GLOB HEADERS src/gecko/*.hpp)
    # install(FILES ${HEADERS} DESTINATION include/gecko)
    #
    # # CppZmq -----------------------------------------------------------
    # install(
    #     FILES extlibs/cppzmq/zmq.hpp extlibs/cppzmq/zmq_addon.hpp
    #     DESTINATION include/gecko/zmq
    # )
    #
    # # MsgPack-Gecko -----------------------------------------------------
    # if(BUILD_MSGPACK)
    #     file(GLOB HEADERS extlibs/gecko-msgpack/gecko/msgpack/*.hpp)
    #     install(FILES ${HEADERS} DESTINATION include/gecko/msgpack)
    # endif()
    #
    # # Json ---------------------------------------------------------------
    # # file(GLOB_RECURSE HEADERS extlibs/nlohmann-json/include/*)
    # # install(FILES ${HEADERS} DESTINATION include/gecko/json)
    # install(
    #     DIRECTORY extlibs/nlohmann-json/include/
    #     DESTINATION include/gecko/json
    # )

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
