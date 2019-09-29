# cmake_minimum_required(VERSION 3.10.0)
# project(gecko-msgpack VERSION 0.5.1 LANGUAGES CXX)
#
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
#
# cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
# # cmake_host_system_information(RESULT CORES QUERY NUMBER_OF_LOGICAL_CORES)
# cmake_host_system_information(RESULT OSN QUERY OS_NAME)
# cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
# cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)
#
# message(STATUS "-------------------------------------")
# message(STATUS "  Project: ${PROJECT_NAME}")
# message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
# message(STATUS "  Path: ${PROJECT_SOURCE_DIR}")
# message(STATUS "-------------------------------------")
# message(STATUS " ${HOST}")
# message(STATUS " ${OSN}: ${OS_VERSION}")
# message(STATUS " ${PROC}")
# message(STATUS "-------------------------------------")
#
# if(1)
#     link_directories(${PROJECT_SOURCE_DIR}/../gecko/build)
#     include_directories(${PROJECT_SOURCE_DIR}/../gecko/src)
#     link_directories(${PROJECT_SOURCE_DIR}/../gecko/build/extlibs/marko)
#     include_directories(${PROJECT_SOURCE_DIR}/../gecko/extlibs/marko/lib/include)
# endif()
#
# include_directories(.)
#
# include_directories(/opt/gecko/include)
# link_directories(/opt/gecko/lib)
# link_directories(/usr/local/lib)
#
# set(CMAKE_PREFIX_PATH "/opt/gecko/lib/cmake;/opt/gecko/share/cmake")
#
# # Load in pkg-config support --------------------------------------
# find_package(PkgConfig)
#
# # Json ------------------------------------------------------------
# # include_directories(extlibs/nlohmann-json/include)
# find_package(nlohmann_json REQUIRED)
# if(nlohmann_json_FOUND)
#     message(STATUS ">> json found: ${nlohmann_json_VERSION}")
# endif()
#
# # Threads ---------------------------------------------------------
# # include(cmake/threads.cmake)
# find_package(Threads REQUIRED)
# if(Threads_FOUND)
#     message(STATUS ">> Threads found")
#     if(CMAKE_USE_PTHREADS_INIT)
#         message(STATUS " -> Using pthreads")
#     endif()
#     list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
# else()
#     message(WARNING "*** Could not find Threads ***")
# endif()
#
# # Marko -----------------------------------------------------------
# # include_directories(extlibs/marko/lib/include)
# # link_directories(extlibs/marko/build)
# # list(APPEND ALL_LIBS "marko")
# # add_subdirectory(extlibs/marko)
#
# # ZeroMQ ----------------------------------------------------------
# # include(cmake/zmq.cmake)
# pkg_check_modules(libzmq REQUIRED libzmq)
# if(libzmq_FOUND)
#     message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
#     include_directories(${libzmq_INCLUDE_DIRS})
#     list(APPEND ALL_LIBS ${libzmq_LIBRARIES})
#
#     option(ZMQ_BUILD_DRAFT_API " " OFF)
# else()
#     message(WARNING "*** Could not find ZeroMQ ***")
# endif()
#
# find_package(cppzmq REQUIRED)
# if(cppzmq_FOUND)
#     message(STATUS ">> cppZMQ found: ${cppzmq_VERSION}")
# endif()

include_directories(.)

# Tests --------------------------------------------------------------
add_subdirectory(gtests)
# add_subdirectory(msgpack)
# add_subdirectory(dev)

message(STATUS "Gecko-MsgPack Tools ----------------------")
list(APPEND tools
    tools/bag/geckobag.cpp
)

# find_package(Boost COMPONENTS system program_options REQUIRED)
# find_package(Boost COMPONENTS system REQUIRED)

foreach(src ${tools})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name}
        PUBLIC
            ${ALL_LIBS}
            gecko
    )

    # install(TARGETS ${name} DESTINATION bin)
endforeach()

# Examples -----------------------------------------------------------
add_subdirectory(example/core)
add_subdirectory(example/json)
add_subdirectory(example/node)
# add_subdirectory(example/opencv)
add_subdirectory(example/simple)
