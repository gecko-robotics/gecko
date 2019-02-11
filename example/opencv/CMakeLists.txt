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
include_directories(/usr/local/include ../../src ../../msgpack)
link_directories(/usr/local/lib ../../build)

find_package(MsgPack REQUIRED)

if(MSGPACK_FOUND)
    include_directories(${MSGPACK_INCLUDE_DIR})
    message(STATUS ">> MsgPack Found: ${MSGPACK_VERSION}")

    list(APPEND ALL_LIBS ${ALL_LIBS} ${MSGPACK_LIBRARIES})

else()
    message(WARNING "\n<<--- Could not find MsgPack --->>\n")
endif()


find_package(ZLIB)
if(ZLIB_FOUND)
    message(STATUS ">> zlib Found: ${ZLIB_VERSION}")
    list(APPEND ALL_LIBS ${ALL_LIBS} ${ZLIB_LIBRARIES})
else()
    message(WARNING "\n<<--- Could not find zlib --->>\n")
endif()

# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found: ${CMAKE_THREAD_VERSION}")
    list(APPEND ALL_LIBS ${ALL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
else()
    message(WARNING "*** Could not find Threads ***")
endif()

# ZeroMQ ----------------------------------------------------------
find_package(ZMQ REQUIRED)
if(ZMQ_FOUND)
    message(STATUS ">> ZeroMQ found: ${ZMQ_VERSION}")
    include_directories(${ZMQ_INCLUDE_DIRS})
    include_directories(../../zmq)
    list(APPEND ALL_LIBS ${ALL_LIBS} ${ZMQ_LIBRARIES})
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

# OpenCV ----------------------------------------------------------
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    list(APPEND ALL_LIBS ${ALL_LIBS} ${OpenCV_LIBS})
else()
    message(WARNING "*** Could not find OpenCV ***")
endif()

# Build Project ----------------------------------------------
file(GLOB SOURCES *.cpp)

list(APPEND ALL_LIBS ${ALL_LIBS} gecko)

foreach(src ${SOURCES})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${ALL_LIBS})
endforeach()
