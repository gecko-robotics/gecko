# cmake_minimum_required(VERSION 3.10.0)
# project(test)
#
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
#
#
# message(STATUS "-------------------------------------")
# message(STATUS "  Project: ${PROJECT_NAME}")
# message(STATUS "-------------------------------------")
#
# # set path for FindXXX.cmake
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
#
# list(APPEND ALL_LIBS "")
#
#
# # Get msgpack -----------------------------------------------------
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)
# # link_directories(../../install/lib)
# # include_directories(../../install/include)
#
# include_directories(
#     /usr/local/include
#     ../../install/include
#     ../../install/include/gecko/zmq
#     ../../install/include/gecko/json
# )
#
# link_directories(
#     /usr/local/lib
#     ../../install/lib
# )
#
# list(APPEND ALL_LIBS "gecko;msgpackc;marko")
#
# find_package(PkgConfig)
# include(../../cmake/msgpack.cmake)
# # include_directories(../../extlibs/gecko-msgpack)  # why????
#
# # Gecko -----------------------------------------------------------
# # include_directories(
# #     ../../src
# #     ../../extlibs/gecko-msgpack)
# # link_directories(../../build)
# #
# # list(APPEND ALL_LIBS "gecko;msgpackc")
#
# # Threads ---------------------------------------------------------
# include(../../cmake/threads.cmake)
#
# # ZeroMQ ----------------------------------------------------------
# # include(../../cmake/zmq.cmake)
# # include_directories(../../extlibs/cppzmq)
#
# # Json ------------------------------------------------------------
# # include_directories(../../extlibs/nlohmann-json/include)
#
# # Marko -----------------------------------------------------------
# # include_directories(../../extlibs/marko/lib/include)
# # link_directories(../../build/extlibs/marko)
# # list(APPEND ALL_LIBS "marko")
#
# # Build Project ----------------------------------------------
# message(STATUS ">> JSON linked libraries: ${ALL_LIBS}")

add_executable(json-pub pub.cpp)
# target_link_libraries(pub ${ALL_LIBS})
# target_link_libraries(json-pub gecko zmq)
target_link_libraries(json-pub ${ALL_LIBS})

# add_executable(sub sub.cpp)
# target_link_libraries(sub ${ALL_LIBS})
