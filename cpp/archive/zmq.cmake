# pkg_check_modules(libzmq REQUIRED libzmq)
find_package(ZeroMQ REQUIRED
    PATHS /opt/gecko/share/cmake
)
# if(libzmq_FOUND)
if (ZeroMQ_FOUND)
    message(STATUS ">> ZeroMQ found: ${ZeroMQ_VERSION}")
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()
