pkg_check_modules(libzmq REQUIRED libzmq)
if(libzmq_FOUND)
    message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
    # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
    # add_compile_definitions(ZMQ_CPP11)
    include_directories(${libzmq_INCLUDE_DIRS})
    # message(STATUS "zmq: ${libzmq_INCLUDE_DIRS}")
    # list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})
    list(APPEND ALL_LIBS ${libzmq_LIBRARIES})

    option(ZMQ_BUILD_DRAFT_API " " OFF)

    # ZeroMQ CPP ------------------------------------------------------
    # git submodule add https://github.com/zeromq/cppzmq
    include_directories(${CMAKE_CURRENT_LIST_DIR}/../extlibs/cppzmq)
    # include_directories(/usr/include)
    # message("oo> ${CMAKE_CURRENT_LIST_DIR}")
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()
