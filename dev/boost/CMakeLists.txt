cmake_minimum_required(VERSION 3.6)
project(boost-test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# include_directories(/usr/local/include)
# link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

list(APPEND ALL_LIBS "")

# Load in pkg-config support ------------------------------------
find_package(PkgConfig)

# CMake ---------------------------------------------------------
include(../../cmake/boost.cmake)

add_executable(port port.cpp)
target_link_libraries(port ${ALL_LIBS})

add_executable(sys2 sys2.cpp)
# target_link_libraries(sys ${Boost_SYSTEM_LIBRARY})
# target_link_libraries(sys ${Boost_LIBRARIES})

add_executable(args args.cpp)
target_link_libraries(args ${ALL_LIBS})

add_executable(hostname hostname.cpp)
target_link_libraries(hostname ${ALL_LIBS})
