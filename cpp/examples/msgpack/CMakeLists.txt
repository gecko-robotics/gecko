# core ---------------------------------------------------------------------
add_executable(core-pub core/pub.cpp)
target_link_libraries(core-pub ${ALL_LIBS})

add_executable(core-sub core/sub.cpp)
target_link_libraries(core-sub ${ALL_LIBS})

# json --------------------------------------------------------------------
add_executable(json-pub json/pub.cpp)
target_link_libraries(json-pub ${ALL_LIBS})

# # simple ------------------------------------------------------------------
# add_executable(pub simple/pub.cpp)
# target_link_libraries(pub ${ALL_LIBS})

# add_executable(sub simple/sub.cpp)
# target_link_libraries(sub ${ALL_LIBS})
