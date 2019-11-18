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

# CMake Libraries ----------------------------------------------------
include(cmake/build.cmake)

# Build Gecko library ----------------------------------------------
file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${ZeroMQ_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        ${marko_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        nlohmann_json::nlohmann_json
        cppzmq
        ${ZeroMQ_INCLUDE_DIR}
        ${marko_INCLUDE_DIRS}
        include
)

# Build tools -------------------------------------------------------
add_subdirectory(tools)

# Tests --------------------------------------------------------------
option(TESTS "gecko google tests" ON)
if(${TESTS})
    add_subdirectory(gtests)
endif()

# Examples -----------------------------------------------------------
option(EXAMPLES "gecko examples" ON)
if(${EXAMPLES})
    add_subdirectory(examples)
endif()

# Install ------------------------------------------------------------
set(GECKO_VERSION "gecko")
set(GECKO_BASE_PATH "/opt/gecko/${GECKO_VERSION}")
set(CMAKE_INSTALL_PREFIX ${GECKO_BASE_PATH})

message(STATUS ">> Setting install base path: ${CMAKE_INSTALL_PREFIX}")

set(GECKO_BIN     "${CMAKE_INSTALL_PREFIX}/bin")
set(GECKO_LIB     "${CMAKE_INSTALL_PREFIX}/lib")
set(GECKO_CMAKE   "${CMAKE_INSTALL_PREFIX}/lib/cmake")
set(GECKO_INCLUDE "${CMAKE_INSTALL_PREFIX}/include/gecko")

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${GECKO_LIB}  # static libraries
  LIBRARY DESTINATION ${GECKO_LIB}  # shared libraries
  RUNTIME DESTINATION ${GECKO_BIN}  # executables
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${GECKO_INCLUDE}
  FILES_MATCHING PATTERN "*.hpp"
  PATTERN ".git" EXCLUDE
)

install(
    FILES
        cmake/build.cmake
        cmake/gecko.cmake
    DESTINATION ${GECKO_CMAKE}/gecko
)

# Package Generator --------------------------------------------------
include(CPack)
if(UNIX AND NOT APPLE)
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
    # SET(CPACK_PACKAGING_INSTALL_PREFIX "/opt/gecko")
    # set(CPACK_PACKAGE_VERSION_MAJOR "0")
    # set(CPACK_PACKAGE_VERSION_MINOR "2")
    # set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_PACKAGE_DESCRIPTION "test")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "test")
    set(CPACK_PACKAGE_CONTACT "Kevin Walchko")
    # set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")
    # set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.html")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
    # set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.html")

    set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")
endif()
