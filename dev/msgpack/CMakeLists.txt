cmake_minimum_required(VERSION 3.6)
project(ssdp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include)
include_directories(../../zmq)
link_directories(/usr/local/lib)

list(APPEND ALL_LIBS "")

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Get msgpack -----------------------------------------------------
pkg_check_modules(MessagePack REQUIRED msgpack)
if(MessagePack_FOUND)
    include_directories(${MessagePack_INCLUDE_DIR})
    message(STATUS ">> MsgPack Found: ${MessagePack_VERSION}")

    list(APPEND ALL_LIBS ${ALL_LIBS} ${MessagePack_LIBRARIES})

    # this is a template library, so no cpp
    include_directories(msgpack)

else()
    message(WARNING "\n<<--- Could not find MsgPack --->>\n")
endif()

# ZeroMQ ----------------------------------------------------------
pkg_check_modules(libzmq REQUIRED libzmq)
if(libzmq_FOUND)
    message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
    # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
    # add_compile_definitions(ZMQ_CPP11)
    include_directories(${libzmq_INCLUDE_DIRS})
    list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})

    # ZeroMQ CPP ------------------------------------------------------
    # https://github.com/zeromq/cppzmq
    include_directories(zmq)
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

# add_executable(mrecv mcreceive.cpp msocket.cpp)
# add_executable(msend mcsend.cpp msocket.cpp)
add_executable(test test.cpp)
target_link_libraries(test ${ALL_LIBS})
