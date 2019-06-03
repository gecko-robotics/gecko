cmake_minimum_required(VERSION 3.6)
project(core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include)
# link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

# set(Boost_DEBUG OFF)

# find_package(Boost COMPONENTS system program_options thread REQUIRED)
# find_package(Boost COMPONENTS system REQUIRED)

# include_directories(${Boost_PROGRAM_OPTIONS_HEADERS})
# message("boost: ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY}")
# message("Version: ${Boost_LIBRARIES}")
# message("headers: ${Boost_INCLUDE_DIRS}")

add_executable(core geckocore.cpp)
# target_link_libraries(core ${Boost_LIBRARIES})
