message(STATUS "Gecko Tools ----------------------")
list(APPEND tools
    logserver/gecko-logserver.cpp
    echo/gecko-echo.cpp
    # bag/gecko-bag.cpp
    core/geckocore.cpp
)

find_package(Boost COMPONENTS system program_options REQUIRED)

foreach(src ${tools})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})

    if (${name} STREQUAL "geckocore")
        target_link_libraries(${name} ${ALL_LIBS} ${Boost_LIBRARIES})
        # message("geckocore !!!!")
    else()
        # message(">> ${name} <<")
        target_link_libraries(${name} ${ALL_LIBS})
    endif()

    install(TARGETS ${name} DESTINATION bin)
endforeach()
