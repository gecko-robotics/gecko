
include(build.cmake)

set(GECKO_FOUND 1)
set(GECKO_VERSION "0.0.0")

list(APPEND gecko_LIBRARIES
    gecko
    ${marko_LIBRARIES}
    ${libzmq_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

list(APPEND gecko_INCLUDE_DIRS
    /opt/gecko/include
    /opt/gecko/gecko/include
    ${marko_INCLUDE_DIRS}
    ${libzmq_INCLUDE_DIRS}
)

message(STATUS "===============================================")
message(STATUS " Gecko ${gecko_VERSION}")
message(STATUS " Exporting gecko_INCLUDE_DIRS gecko_LIBRARIES")
