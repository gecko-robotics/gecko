# core ---------------------------------------------------------------------
add_executable(core-pub core/pub.cpp)
target_link_libraries(core-pub ${ALL_LIBS})

add_executable(core-sub core/sub.cpp)
target_link_libraries(core-sub ${ALL_LIBS})

# json --------------------------------------------------------------------
add_executable(json-pub json/pub.cpp)
target_link_libraries(json-pub ${ALL_LIBS})

# simple ------------------------------------------------------------------
add_executable(pub simple/pub.cpp)
target_link_libraries(pub ${ALL_LIBS})

add_executable(sub simple/sub.cpp)
target_link_libraries(sub ${ALL_LIBS})

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
