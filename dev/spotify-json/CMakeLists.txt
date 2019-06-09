cmake_minimum_required(VERSION 3.10)
project(json)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include)

# boost locations
# export BOOST_ROOT=/usr/include
# export BOOST_LIBRARYDIR=/usr/lib/x86_64-linux-gnu

include_directories(./spotify-json-master/vendor/double-conversion)
link_directories(./spotify-json-master/build/vendor/double-conversion/double-conversion)
include_directories(./spotify-json-master/include)
link_directories(./spotify-json-master/build)

# set(Boost_DEBUG OFF)

find_package(Boost COMPONENTS system program_options thread REQUIRED)
# find_package(Boost COMPONENTS system thread REQUIRED)
# find_package(Boost COMPONENTS system REQUIRED)

include_directories(${Boost_PROGRAM_OPTIONS_HEADERS})
message("boost: ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY}")
message("Version: ${Boost_LIBRARIES}")
message("headers: ${Boost_INCLUDE_DIRS}")

add_executable(json test-json.cpp)
target_link_libraries(json ${Boost_LIBRARIES} double-conversion spotify-json)

# add_executable(sys2 sys2.cpp)
# target_link_libraries(sys ${Boost_SYSTEM_LIBRARY})
# target_link_libraries(sys ${Boost_LIBRARIES})

# add_executable(args args.cpp)
# target_link_libraries(args ${Boost_LIBRARIES})
#
# add_executable(hostname hostname.cpp)
# target_link_libraries(hostname ${Boost_LIBRARIES})
