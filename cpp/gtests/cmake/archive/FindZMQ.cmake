# Look for the header file.
FIND_PATH(ZMQ_INCLUDE_DIR NAMES zmq.h PATHS
    /usr/include
    /usr/local/include
)

# Look for the library.
FIND_LIBRARY(ZMQ_LIBRARY NAMES libzmq zmq PATHS
    /usr/lib
    /usr/local/lib
)

SET(ZMQ_LIBRARIES ${ZMQ_LIBRARY})
SET(ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})

if (${ZMQ_LIBRARY} STREQUAL "")
    set(MSG "Could not find ZMQ library")
    set(ZMQ_FOUND FALSE)
    if (ZMQ_FIND_REQUIRED)
        message(FATAL_ERROR ${MSG})
    else()
        message(WARNING ${MSG})
    endif()
else()
    set(ZMQ_FOUND TRUE)
    # message(WARNING "found zmq")
endif()



INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZMQ DEFAULT_MSG ZMQ_LIBRARY ZMQ_INCLUDE_DIR)
MARK_AS_ADVANCED(ZMQ_INCLUDE_DIR ZMQ_LIBRARY)
