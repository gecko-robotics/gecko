message(STATUS "Gecko Tools ----------------------")
list(APPEND tools
    logserver/gecko-logserver.cpp
    topic/geckotopic.cpp
    # bag/gecko-bag.cpp
    core/geckocore.cpp
    version/version.cpp
)

# find_package(Boost COMPONENTS system program_options REQUIRED)
# find_package(Boost COMPONENTS system REQUIRED)

foreach(src ${tools})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    add_executable(${name} ${src})

    # if (${name} STREQUAL "geckocore")
    #     target_link_libraries(${name} ${ALL_LIBS} ${Boost_LIBRARIES})
    #     # message("geckocore !!!!")
    # else()
        # message(">> ${name} <<")
        target_link_libraries(${name} ${ALL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
    # endif()

    install(TARGETS ${name} DESTINATION bin)
endforeach()
