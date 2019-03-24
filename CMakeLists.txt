cmake_minimum_required(VERSION 3.10.0)
project(gecko)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

# set path for FindXXX.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND ALL_LIBS "")

# message(STATUS $ENV{HOME})

# Add local -----------------------------------------------------
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

if (NOT APPLE)
    set(msgpack_DIR $ENV{HOME}/.local/lib/cmake/msgpack)
    include_directories($ENV{HOME}/.local/include)
    link_directories($ENV{HOME}/.local/lib)
endif()

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

# Get zlib -----------------------------------------------------
find_package(ZLIB)
if(ZLIB_FOUND)
    message(STATUS ">> zlib Found: ${ZLIB_VERSION_STRING}")
    message(STATUS ">> zlib Found: ${ZLIB_LIBRARIES}")
    list(APPEND ALL_LIBS ${ZLIB_LIBRARIES})
else()
    message(WARNING "\n<<--- Could not find zlib --->>\n")
endif()

# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
    list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
else()
    message(WARNING "*** Could not find Threads ***")
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

# Build Gecko library ----------------------------------------------
file(GLOB SOURCES src/*.cpp)
include_directories(src)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
list(APPEND ALL_LIBS ${PROJECT_NAME})


# Tests --------------------------------------------------------------
add_subdirectory(tests)
add_subdirectory(gtests)
add_subdirectory(msgpack)
add_subdirectory(tools)

# Tests --------------------------------------------------------------
# list(APPEND tests
#     # tests/core-test.cpp
#     tests/zmq-test.cpp
#     tests/time-test.cpp
#     tests/ip-test.cpp
#     # tests/proc-test.cpp
#     tests/node-test.cpp
#     tests/node-2-test.cpp
#     tests/node-3-test.cpp
#     tests/node-4-test.cpp
#     tests/node-5-test.cpp
#     tests/gecko-test.cpp
#     tests/gecko-2-test.cpp
#     tests/zmq-message-test.cpp
#     tests/zmq-interrupt-test.cpp
#     tests/simple-zmq-test.cpp
#     # tests/directory-test.cpp
#     # tests/thread-test.cpp
#     tests/color-test.cpp
#     # tests/multicast-test.cpp
#     # tests/zmq-req-test.cpp
#     tests/process-test.cpp
#     tests/process-2-test.cpp
# )
#
# message(STATUS "Gecko Tests ----------------------")
# foreach(src ${tests})
#     get_filename_component(name ${src} NAME_WE)
#     message(STATUS " -> ${name}")
#     add_executable(${name} ${src})
#     target_link_libraries(${name} ${ALL_LIBS})
# endforeach()


# list(APPEND msgpktests
#     tests/msgpack-test.cpp
#     # tests/msgpack-2-test.cpp
#     # tests/msgpack-3-test.cpp
#     tests/msgpack-4-test.cpp
#     tests/msgpack-5-test.cpp
#     tests/msgpack-6-test.cpp
#     tests/msgpack-7-test.cpp
#     tests/msgpack-8-test.cpp
#     # tests/msgpack-zlib-test.cpp
#     # tests/zlib-test.cpp
#     # tests/multicast-test.cpp
#     # tests/multicast-2-test.cpp
# )
#
# set(MSGPACK_ALL
#     ${PROJECT_NAME}
#     ${MSGPACK_LIBRARY}
#     ${CMAKE_THREAD_LIBS_INIT}
#     ${ZMQ_LIBRARIES}
# )
#
# message(STATUS "MsgPack Tests ----------------------")
# foreach(src ${msgpktests})
#     get_filename_component(name ${src} NAME_WE)
#     message(STATUS " -> ${name}")
#     add_executable(${name} ${src})
#     target_link_libraries(${name} ${MSGPACK_ALL})
# endforeach()


# message(STATUS "Gecko Tools ----------------------")
# list(APPEND tools
#     tools/logserver/logserver.cpp
#     tools/echo/echo.cpp
# )
#
# foreach(src ${tools})
#     get_filename_component(name ${src} NAME_WE)
#     message(STATUS " -> ${name}")
#     add_executable(${name} ${src})
#     target_link_libraries(${name} ${ALL_LIBS})
# endforeach()

# #############################################################################
# ##  gTest
# ##
# ## Code below taken from README
# #############################################################################
# # Download and unpack googletest at configure time
# configure_file(cmake/CMakeLists.txt.in googletest-download/CMakeLists.txt)
# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "CMake step for googletest failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "Build step for googletest failed: ${result}")
# endif()
#
# # Prevent overriding the parent project's compiler/linker
# # settings on Windows
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#
# # Add googletest directly to our build. This defines
# # the gtest and gtest_main targets.
# add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
#                  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
#                  EXCLUDE_FROM_ALL)
#
# # The gtest/gtest_main targets carry header search path
# # dependencies automatically when using CMake 2.8.11 or
# # later. Otherwise we have to add them here ourselves.
# if (CMAKE_VERSION VERSION_LESS 2.8.11)
#   include_directories("${gtest_SOURCE_DIR}/include")
# endif()

# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(gtest-example gtests/example.cpp)
# target_link_libraries(gtest-example gtest_main)
# add_test(NAME example_test COMMAND gtest-example)

# list(APPEND gtests
#     gtests/example-gtest.cpp
#     gtests/zmq-gtest.cpp
#     gtests/ascii-gtest.cpp
#     gtests/msgpack-gtest.cpp
# )
#
# message(STATUS "gTests ----------------------")
# foreach(src ${gtests})
#     get_filename_component(name ${src} NAME_WE)
#     message(STATUS " -> ${name}")
#     add_executable(${name} ${src})
#     target_link_libraries(${name} gtest_main ${MSGPACK_ALL})
#     add_test(NAME ${name} COMMAND ${name})
# endforeach()
