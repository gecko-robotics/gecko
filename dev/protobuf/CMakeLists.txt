
find_package(Protobuf REQUIRED)
if ( Protobuf_FOUND )
    include_directories(${Protobuf_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
    message( STATUS "Protobuf compiler libraries : ${Protobuf_PROTOC_LIBRARIES}")
    message( STATUS "Protobuf lite libraries : ${Protobuf_LITE_LIBRARIES}")

    set(
        MSG_PROTOS
        msgs/imu.proto
        msgs/vector.proto
        msgs/image.proto
        msgs/rep_req.proto
    )
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${MSG_PROTOS})
else()
    message( WARNING "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
endif()


set(ALL_LIBS
    ${Protobuf_LIBRARIES}
    ${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ZMQ_LIBRARIES}
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(
    ${PROJECT_NAME}
    ${Protobuf_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ZMQ_LIBRARIES}
)
