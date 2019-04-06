message(STATUS "Gecko Tools ----------------------")
list(APPEND tools
    logserver/gecko-logserver.cpp
    echo/gecko-echo.cpp
    bag/gecko-bag.cpp
)

foreach(src ${tools})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})
    target_link_libraries(${name} ${ALL_LIBS})

    install(TARGETS ${name} DESTINATION bin)
endforeach()
