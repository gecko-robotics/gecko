cmake_minimum_required(VERSION 3.6)
project(ssdp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include)
# link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

set(Boost_DEBUG OFF)

find_package(Boost 1.65.1 COMPONENTS system  REQUIRED)

include_directories(${Boost_FILESYSTEM_HEADERS})
message("boost: ${Boost_FILE_SYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_INCLUDE_DIRS}")
message("Version: ${Boost_LIB_VERSION}")

add_executable(port port.cpp)
target_link_libraries(port ${Boost_SYSTEM_LIBRARY})


add_executable(sys sys.cpp)
target_link_libraries(sys ${Boost_SYSTEM_LIBRARY})
# target_link_libraries(sys ${Boost_SYSTEM_LIBRARY} boost_file_system)
