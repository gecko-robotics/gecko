# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
    # list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
else()
    message(WARNING "*** Could not find Threads ***")
endif()

# Marko -----------------------------------------------------------
include(/opt/gecko/lib/cmake/marko/markoConfig.cmake)
if(marko_FOUND)
    message(STATUS ">> marko found: ${marko_VERSION}")
endif()

# Json ------------------------------------------------------------
find_package(nlohmann_json REQUIRED
    PATHS /opt/gecko/lib/cmake
)
if(nlohmann_json_FOUND)
    message(STATUS ">> json found: ${nlohmann_json_VERSION}")
endif()

# ZeroMQ ----------------------------------------------------------
find_package(ZeroMQ REQUIRED
    PATHS /opt/gecko/share/cmake
)
if (ZeroMQ_FOUND)
    message(STATUS ">> ZeroMQ found: ${ZeroMQ_VERSION}")
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()

find_package(cppzmq REQUIRED
    PATHS /opt/gecko/share/cmake
)
if(cppzmq_FOUND)
    message(STATUS ">> cppZMQ found: ${cppzmq_VERSION}")
endif()
