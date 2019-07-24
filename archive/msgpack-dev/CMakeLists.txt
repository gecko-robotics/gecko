cmake_minimum_required(VERSION 3.6)
project(ssdp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add local -----------------------------------------------------
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

if (NOT APPLE)
    set(msgpack_DIR $ENV{HOME}/.local/lib/cmake/msgpack)
    include_directories($ENV{HOME}/.local/include)
    link_directories($ENV{HOME}/.local/lib)
endif()

include_directories(/usr/local/include)
include_directories(../../zmq)
link_directories(../../build)
include_directories(../../src)
# include_directories(../../msgpack)
link_directories(/usr/local/lib)

list(APPEND ALL_LIBS "gecko")

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Get msgpack -----------------------------------------------------
if(APPLE)
  pkg_check_modules(MessagePack REQUIRED msgpack)
  if(MessagePack_FOUND)
      include_directories(${MessagePack_INCLUDE_DIR})
      message(STATUS ">> MsgPack Found: ${MessagePack_VERSION}")
      message(STATUS ">> MsgPack: ${MessagePack_LIBRARIES}  ${MessagePack_INCLUDE_DIR}")
      list(APPEND ALL_LIBS ${ALL_LIBS} ${MessagePack_LIBRARIES})

      # this is a template library, so no cpp
      include_directories(msgpack)

  else()
      message(WARNING "\n<<--- Could not find MsgPack --->>\n")
  endif()
elseif(UNIX)
    find_package(msgpack REQUIRED)
    if(msgpack_FOUND)
        include_directories(${msgpack_INCLUDE_DIR})
        message(STATUS ">> MsgPack Found: ${msgpack_VERSION}")
        message(STATUS ">> MsgPack: ${msgpack_LIBS}  ${msgpack_INCLUDE_DIR}")
        list(APPEND ALL_LIBS ${msgpack_LIBRARIES})

        # this is a template library, so no cpp
        include_directories(msgpack)

    else()
        message(WARNING "\n<<--- Could not find MsgPack --->>\n")
    endif()
endif()

# ZeroMQ ----------------------------------------------------------
pkg_check_modules(libzmq REQUIRED libzmq)
if(libzmq_FOUND)
    message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
    # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
    # add_compile_definitions(ZMQ_CPP11)
    include_directories(${libzmq_INCLUDE_DIRS})
    # list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})
    list(APPEND ALL_LIBS ${libzmq_LIBRARIES})

    # ZeroMQ CPP ------------------------------------------------------
    # https://github.com/zeromq/cppzmq
    include_directories(zmq)
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

# add_executable(mrecv mcreceive.cpp msocket.cpp)
# add_executable(msend mcsend.cpp msocket.cpp)
add_executable(test-msg test.cpp)
target_link_libraries(test-msg ${ALL_LIBS})
