
list(APPEND msgpktests
    tests/msgpack-test.cpp
    # tests/msgpack-2-test.cpp
    # tests/msgpack-3-test.cpp
    tests/msgpack-4-test.cpp
    tests/msgpack-5-test.cpp
    tests/msgpack-6-test.cpp
    tests/msgpack-7-test.cpp
    tests/msgpack-8-test.cpp
    # tests/msgpack-9-test.cpp
    # tests/msgpack-zlib-test.cpp
    # tests/zlib-test.cpp
    # tests/multicast-test.cpp
    # tests/multicast-2-test.cpp
)

set(MSGPACK_ALL
    ${PROJECT_NAME}
    ${MSGPACK_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ZMQ_LIBRARIES}
)

message(STATUS "MsgPack Tests ----------------------")
foreach(src ${msgpktests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${MSGPACK_ALL})
endforeach()
