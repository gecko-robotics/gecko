cmake_minimum_required(VERSION 3.10.0)
project(gecko)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_C_COMPILER /usr/local/bin/gcc-8)
# set(CMAKE_CXX_COMPILER /usr/local/bin/g++-8)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "-------------------------------------")

# set path for FindXXX.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

list(APPEND ALL_LIBS "")

# find_package(Protobuf REQUIRED)
# if ( Protobuf_FOUND )
#     include_directories(${Protobuf_INCLUDE_DIRS})
#     include_directories(${CMAKE_CURRENT_BINARY_DIR})
#     message( STATUS ">> Protobuf Found: ${Protobuf_VERSION}" )
#     # message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
#     # message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
#     # message( STATUS "Protobuf compiler libraries : ${Protobuf_PROTOC_LIBRARIES}")
#     # message( STATUS "Protobuf lite libraries : ${Protobuf_LITE_LIBRARIES}")
#
#     list(APPEND ALL_LIBS ${ALL_LIBS} ${Protobuf_LIBRARIES})
#
#     set(
#         MSG_PROTOS
#         protobuf/imu.proto
#         protobuf/vector.proto
#         protobuf/image.proto
#         protobuf/rep_req.proto
#     )
#     protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${MSG_PROTOS})
# else()
#     # message(WARNING "*******************************")
#     message(WARNING "*** Could not find Protobuf ***")
#     # message(WARNING "*******************************")
# endif()


# Get msgpack -----------------------------------------------------
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
# include_directories(/usr/local/opt/libunwind-headers/include)
# find_package(MsgPack)

# include(FindMsgPack.cmake)
find_package(MsgPack REQUIRED)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
# "/usr/local/lib/cmake")
# set(CMAKE_MODULE_PATH "/usr/local/lib/cmake")
# find_package(msgpack) # msgpack_found not set!!!!
if(MSGPACK_FOUND)
    include_directories(${MSGPACK_INCLUDE_DIR})
    message(STATUS ">> MsgPack Found: ${MSGPACK_VERSION}")

    list(APPEND ALL_LIBS ${ALL_LIBS} ${MSGPACK_LIBRARIES})

else()
    message(WARNING "\n<<--- Could not find MsgPack --->>\n")
endif()

# message(STATUS "+++ ${MSGPACK_LIBRARY}")
# message(STATUS ">> MsgPack Found: ${MSGPACK_VERSION}")


find_package(ZLIB)
if(ZLIB_FOUND)
    message(STATUS ">> zlib Found: ${ZLIB_VERSION}")
    list(APPEND ALL_LIBS ${ALL_LIBS} ${ZLIB_LIBRARIES})
else()
    message(WARNING "\n<<--- Could not find zlib --->>\n")
endif()

# Add local directories -------------------------------------------
include_directories(src)

# ZeroMQ CPP ------------------------------------------------------
# https://github.com/zeromq/cppzmq
include_directories(zmq)

# Load in pkg-config support --------------------------------------
# find_package(PkgConfig)

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
    # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
    # add_compile_definitions(ZMQ_CPP11)
    include_directories(${ZMQ_INCLUDE_DIRS})
    list(APPEND ALL_LIBS ${ALL_LIBS} ${ZMQ_LIBRARIES})
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

# Build Gecko library ----------------------------------------------
file(GLOB SOURCES src/*.cpp)

if(MSGPACK_FOUND)
    # list(APPEND SOURCES
    #     ${SOURCES}
    #     # msgpack/transport.cpp
    #     # msgpack/transport-impl.cpp
    # )

    # this is a template library, so no cpp
    include_directories(msgpack)
endif()

# add_library(${PROJECT_NAME} SHARED ${SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

list(APPEND ALL_LIBS ${ALL_LIBS} ${PROJECT_NAME})

# Tests --------------------------------------------------------------
list(APPEND tests
    tests/core-test.cpp
    tests/zmq-test.cpp
    tests/time-test.cpp
    tests/ip-test.cpp
    tests/proc-test.cpp
    tests/node-test.cpp
    tests/gecko-test.cpp
    tests/zmq-message-test.cpp
    tests/simple-zmq-test.cpp
    # tests/multicast-test.cpp
    # tests/zmq-req-test.cpp
)

message(STATUS "Gecko Tests ----------------------")
foreach(src ${tests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${ALL_LIBS})
endforeach()

list(APPEND msgpktests
    tests/msgpack-test.cpp
    tests/msgpack-2-test.cpp
    # tests/msgpack-3-test.cpp
    tests/msgpack-4-test.cpp
    tests/msgpack-5-test.cpp
    tests/msgpack-6-test.cpp
    tests/msgpack-7-test.cpp
    tests/msgpack-8-test.cpp
    tests/msgpack-zlib-test.cpp
    tests/zlib-test.cpp
    tests/multicast-test.cpp
)

set(MSGPACK_ALL
    ${PROJECT_NAME}
    ${MSGPACK_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ZMQ_LIBRARIES}
)

message(STATUS "MsgPack Tests ----------------------")
foreach(src ${msgpktests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${MSGPACK_ALL})
endforeach()

#############################################################################
##  gTest
##
## Code below taken from README
#############################################################################
# Download and unpack googletest at configure time
configure_file(cmake/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(gtest-example gtests/example.cpp)
# target_link_libraries(gtest-example gtest_main)
# add_test(NAME example_test COMMAND gtest-example)

list(APPEND gtests
    gtests/example-gtest.cpp
)

message(STATUS "gTests ----------------------")
foreach(src ${gtests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} gtest_main)
    add_test(NAME example_test COMMAND ${name})
endforeach()
