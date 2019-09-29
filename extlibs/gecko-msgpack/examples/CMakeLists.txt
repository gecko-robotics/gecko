# core ---------------------------------------------------------------------
add_executable(core-pub-mp core/pub.cpp)
target_link_libraries(core-pub-mp ${ALL_LIBS})

add_executable(core-sub-mp core/sub.cpp)
target_link_libraries(core-sub-mp ${ALL_LIBS})

# json --------------------------------------------------------------------
add_executable(json-pub-mp json/pub.cpp)
target_link_libraries(json-pub-mp ${ALL_LIBS})

# simple ------------------------------------------------------------------
add_executable(pub-mp simple/pub.cpp)
target_link_libraries(pub-mp ${ALL_LIBS})

add_executable(sub-mp simple/sub.cpp)
target_link_libraries(sub-mp ${ALL_LIBS})

# OpenCV ----------------------------------------------------------
if(0)
    find_package(OpenCV REQUIRED)
    if(OpenCV_FOUND)
        message(STATUS ">> OpenCV found: ${OpenCV_VERSION}")
    else()
        message(WARNING "*** Could not find OpenCV ***")
    endif()

    # Build Project ----------------------------------------------
    file(GLOB SOURCES opencv/*.cpp)

    list(APPEND ALL_LIBS ${ALL_LIBS} ${OpenCV_LIBS} gecko)

    foreach(src ${SOURCES})
        get_filename_component(name ${src} NAME_WE)
        message(STATUS " -> ${name}")
        add_executable(opencv-${name} ${src})
        target_link_libraries(opencv-${name} ${ALL_LIBS})
    endforeach()
endif()
