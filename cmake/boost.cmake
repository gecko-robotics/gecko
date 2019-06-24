set(Boost_DEBUG OFF)

include_directories(/usr/local/include)
find_package(Boost COMPONENTS system program_options thread REQUIRED)
# find_package(Boost COMPONENTS system thread REQUIRED)
# find_package(Boost COMPONENTS system REQUIRED)

include_directories(${Boost_PROGRAM_OPTIONS_HEADERS})
# message("boost: ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY}")
# message("Version: ${Boost_LIBRARIES}")
# message("headers: ${Boost_INCLUDE_DIRS}")

list(APPEND ALL_LIBS "${Boost_LIBRARIES}")
