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
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND ALL_LIBS "")

# Add local -----------------------------------------------------
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

# if (NOT APPLE)
#     set(msgpack_DIR $ENV{HOME}/.local/lib/cmake/msgpack)
#     include_directories($ENV{HOME}/.local/include)
#     link_directories($ENV{HOME}/.local/lib)
# endif()

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

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
    # message(STATUS "zmq: ${libzmq_INCLUDE_DIRS}")
    # list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})
    list(APPEND ALL_LIBS ${libzmq_LIBRARIES})

    # ZeroMQ CPP ------------------------------------------------------
    # https://github.com/zeromq/cppzmq
    include_directories(zmq)
    # include_directories(/usr/include)
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

# Build Gecko library ----------------------------------------------
file(GLOB SOURCES src/*.cpp)
include_directories(src)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
list(APPEND ALL_LIBS ${PROJECT_NAME})

# Package Generator ------------------
if(APPLE)
    set(CPACK_GENERATOR "DMG")
    # PackageMaker (Mac OSX) packaging.
    # find_program(PACKAGE_MAKER "PackageMaker")
    # if(PACKAGE_MAKER)
    #     set(CPACK_GENERATOR "PackageMaker")
    # endif()
else()
    set(CPACK_GENERATOR "DEB")
    # DEB (Debian, Ubuntu) packaging.
    find_program(DPKG "dpkg")
    if(DPKG)
        set(CPACK_GENERATOR "DEB")
        if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
        elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
        elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armhf")  # RPi
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
        endif()
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Me <support@example.com>")
        set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    endif()
    # set(CPACK_SOURCE_GENERATOR "TGZ")
endif()
SET(CPACK_PACKAGING_INSTALL_PREFIX "/home/kevin/.local")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "msgpack")
set(CPACK_PACKAGE_DESCRIPTION "test")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "test")
set(CPACK_PACKAGE_CONTACT "Kevin Walchko")
# set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.html")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
# set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.html")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

include(CPack)


# Tests --------------------------------------------------------------
add_subdirectory(tests)
add_subdirectory(gtests)
add_subdirectory(tools)

# Install ----------
# You can redefine at build time:
#   cmake .. -DCMAKE_INSTALL_PREFIX=/home/kevin/.local
# To create debian package, run: make package
#
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

file(GLOB HEADERS src/gecko/*.hpp)
install(FILES ${HEADERS} DESTINATION include/gecko)
install(FILES zmq/zmq.hpp zmq/zmq_addon.hpp DESTINATION include/gecko/zmq)
