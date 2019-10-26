# cmake_minimum_required(VERSION 3.6)
# project(marko)
#
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
#
# message(STATUS "-------------------------------------")
# message(STATUS "  Project: ${PROJECT_NAME}")
# message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
# message(STATUS "-------------------------------------")

# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

# file(GLOB SOURCES lib/*.cpp)
#
# add_library(marko SHARED ${SOURCES})
# target_include_directories(marko
#     PUBLIC
#         lib/include
# )

# include_directories(lib/include)

# add_executable(request request.cpp)
# target_link_libraries(request ${PROJECT_NAME})
#
# add_executable(reply reply.cpp)
# target_link_libraries(reply ${PROJECT_NAME})

# Protobuf ------------------------------------------------------
find_package(Protobuf REQUIRED)
if ( Protobuf_FOUND )
    # include_directories(${Protobuf_INCLUDE_DIRS})
    # include_directories(${CMAKE_CURRENT_BINARY_DIR})
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
    message( STATUS "Protobuf compiler libraries : ${Protobuf_PROTOC_LIBRARIES}")
    message( STATUS "Protobuf lite libraries : ${Protobuf_LITE_LIBRARIES}")

    # set(BASE_MSG "./")
    # set(MSG_PROTOS
    #     # ${BASE_MSG}/standard_msgs.proto
    #     # ${BASE_MSG}/nav_msgs.proto
    #     # ${BASE_MSG}/sensor_msgs.proto
    #     ${BASE_MSG}/msgs.proto
    # )
    set(CWD ${CMAKE_CURRENT_SOURCE_DIR})
    message(STATUS ">>> ${CWD}")
    execute_process(COMMAND "protoc" "--proto_path=./ --cpp_out=./ msgs.proto"
        WORKING_DIRECTORY ${CWD}
    )
    # protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${MSG_PROTOS})
    # execute_process(COMMAND

    # message(STATUS "[>>] ${PROTO_SRCS} \n${PROTO_HDRS} \n${BASE_MSG}/${MSG_PROTOS}")

    # add_library(proto SHARED ${PROTO_SRCS} ${PROTO_HDRS})
    file(GLOB PROTO_SRCS *.pb.cc)
    file(GLOB PROTO_HDRS *.pb.h helper_pb.h)
    add_library(proto
        SHARED
            ${PROTO_SRCS}
            ${PROTO_HDRS}
    )

    target_include_directories(proto
        PUBLIC
            ./
            ${CMAKE_CURRENT_BINARY_DIR}
            ${Protobuf_INCLUDE_DIRS}
    )

    target_link_libraries(proto
        PUBLIC
            ${Protobuf_LIBRARIES}
    )

    list(APPEND ALL_LIBS proto)

    install(
        FILES ${PROTO_HDRS}
        DESTINATION include/gecko/protobuf
    )
    install(
        TARGETS proto
        DESTINATION lib
    )

else()
    message( WARNING "Protobuf package not found -> set PROTOBUF_ROOT variable")
endif()
#
# install(
#     DIRECTORY lib/include/
#     DESTINATION include/gecko/marko
# )
# install(
#     TARGETS marko
#     DESTINATION lib
# )
