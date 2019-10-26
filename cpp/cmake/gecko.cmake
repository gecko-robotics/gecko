
find_package(PkgConfig)

# find_package(Protobuf 3 REQUIRED)
# if ( Protobuf_FOUND )
#     message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
# else()
#     message( FATAL_ERROR "Protobuf package not found -> specify PROTOBUF_ROOT variable")
# endif()

pkg_check_modules(libzmq REQUIRED libzmq)
if(libzmq_FOUND)
    message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
else()
    message(FATAL_ERROR "*** Could not find ZeroMQ ***")
endif()

find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
    list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
else()
    message(FATAL_ERROR "*** Could not find Threads ***")
endif()

list(APPEND GECKO_LIBS
    # proto
    gecko
    marko
    ${libzmq_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    # ${Protobuf_LIBRARIES}
)

list(APPEND GECKO_INCLUDES
    /opt/gecko/include
    /opt/gecko/gecko/include
    /opt/gecko/gecko/include/gecko/marko
    ${libzmq_INCLUDE_DIRS}
)
