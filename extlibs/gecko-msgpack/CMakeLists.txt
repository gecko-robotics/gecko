include_directories(.)

# Tests --------------------------------------------------------------
add_subdirectory(gtests)

message(STATUS "Gecko-MsgPack Tools ----------------------")
list(APPEND tools
    tools/bag/geckobag.cpp
)

foreach(src ${tools})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name}
        PUBLIC
            ${ALL_LIBS}
            gecko
    )

    # install(TARGETS ${name} DESTINATION bin)
endforeach()

# Examples -----------------------------------------------------------
# add_subdirectory(example/core)
# add_subdirectory(example/json)
# add_subdirectory(example/node)
# add_subdirectory(example/opencv)
# add_subdirectory(example/simple)
