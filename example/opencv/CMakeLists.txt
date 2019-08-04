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
# # Load in pkg-config support --------------------------------------
# find_package(PkgConfig)
#
#
# # Add local -----------------------------------------------------
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)
#
# # Get msgpack -----------------------------------------------------
# include_directories(/usr/local/include ../../src ../../msgpack)
# link_directories(/usr/local/lib ../../build)
#
# # find_package(MsgPack REQUIRED)
# #
# # if(MSGPACK_FOUND)
# #     include_directories(${MSGPACK_INCLUDE_DIR})
# #     message(STATUS ">> MsgPack Found: ${MSGPACK_VERSION}")
# #
# #     list(APPEND ALL_LIBS ${ALL_LIBS} ${MSGPACK_LIBRARIES})
# #
# # else()
# #     message(WARNING "\n<<--- Could not find MsgPack --->>\n")
# # endif()
#
# # Get msgpack -----------------------------------------------------
# pkg_check_modules(MessagePack REQUIRED msgpack)
# if(MessagePack_FOUND)
#     include_directories(${MessagePack_INCLUDE_DIR})
#     message(STATUS ">> MsgPack Found: ${MessagePack_VERSION}")
#
#     list(APPEND ALL_LIBS ${ALL_LIBS} ${MessagePack_LIBRARIES})
#
#     # this is a template library, so no cpp
#     include_directories(msgpack)
# else()
#     message(WARNING "\n<<--- Could not find MsgPack --->>\n")
# endif()
#
# find_package(ZLIB)
# if(ZLIB_FOUND)
#     message(STATUS ">> zlib Found: ${ZLIB_VERSION_STRING}")
#     list(APPEND ALL_LIBS ${ALL_LIBS} ${ZLIB_LIBRARIES})
# else()
#     message(WARNING "\n<<--- Could not find zlib --->>\n")
# endif()
#
# # Threads ---------------------------------------------------------
# find_package(Threads REQUIRED)
# if(Threads_FOUND)
#     message(STATUS ">> Threads found")
#     if(CMAKE_USE_PTHREADS_INIT)
#         message(STATUS " -> Using pthreads")
#     endif()
#     list(APPEND ALL_LIBS ${ALL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
# else()
#     message(WARNING "*** Could not find Threads ***")
# endif()
#
# # ZeroMQ ----------------------------------------------------------
# pkg_check_modules(libzmq REQUIRED libzmq)
# if(libzmq_FOUND)
#     message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
#     # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
#     # add_compile_definitions(ZMQ_CPP11)
#     include_directories(${libzmq_INCLUDE_DIRS})
#     list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})
#
#     # ZeroMQ CPP ------------------------------------------------------
#     # https://github.com/zeromq/cppzmq
#     include_directories(../../zmq)
# else()
#     message(WARNING "*** Could not find ZeroMQ ***")
# endif()

# OpenCV ----------------------------------------------------------
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message(STATUS ">> OpenCV found: ${OpenCV_VERSION}")
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
    add_executable(opencv-${name} ${src})
    target_link_libraries(opencv-${name} ${ALL_LIBS})
endforeach()
