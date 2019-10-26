# simple ------------------------------------------------------------------
add_executable(pub_pb simple/pub.cpp)
target_link_libraries(pub_pb ${ALL_LIBS} proto)

add_executable(sub_pb simple/sub.cpp)
target_link_libraries(sub_pb ${ALL_LIBS} proto)
