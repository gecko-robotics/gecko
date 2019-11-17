cmake_minimum_required(VERSION 3.10)
project(marko)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

# Library =====================================================================
set(SOURCES src/ascii.cpp src/bsocket.cpp src/mcsocket.cpp src/ssocket.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ./include
)

# Examples ====================================================================
option(EXAMPLES "Simple examples" ON)
if (EXAMPLES)
    add_subdirectory(examples)
endif()

# Install =====================================================================
set(MARKO_BASE_PATH "/opt/gecko")
set(CMAKE_INSTALL_PREFIX ${MARKO_BASE_PATH})
# ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(MARKO_BIN     "${MARKO_BASE_PATH}/bin")
set(MARKO_LIB     "${MARKO_BASE_PATH}/lib")
set(MARKO_CMAKE   "${MARKO_BASE_PATH}/lib/cmake")
set(MARKO_INCLUDE "${MARKO_BASE_PATH}/include/marko")

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${MARKO_LIB}  # static libraries
  LIBRARY DESTINATION ${MARKO_LIB}  # shared libraries
  RUNTIME DESTINATION ${MARKO_BIN}  # executables
)

install(DIRECTORY include/marko/
  DESTINATION ${MARKO_INCLUDE}
  FILES_MATCHING PATTERN "*.hpp"
  # PATTERN ".git" EXCLUDE
)

install(
    FILES cmake/markoConfig.cmake
    DESTINATION ${MARKO_CMAKE}/marko
)
