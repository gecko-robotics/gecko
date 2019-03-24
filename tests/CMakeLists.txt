
list(APPEND tests
    # core-test.cpp
    zmq-test.cpp
    time-test.cpp
    ip-test.cpp
    # proc-test.cpp
    node-test.cpp
    node-2-test.cpp
    node-3-test.cpp
    node-4-test.cpp
    node-5-test.cpp
    gecko-test.cpp
    gecko-2-test.cpp
    zmq-message-test.cpp
    zmq-interrupt-test.cpp
    simple-zmq-test.cpp
    # directory-test.cpp
    # thread-test.cpp
    color-test.cpp
    # multicast-test.cpp
    # zmq-req-test.cpp
    process-test.cpp
    process-2-test.cpp
)

message(STATUS "Gecko Tests ----------------------")
foreach(src ${tests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${ALL_LIBS})
endforeach()
