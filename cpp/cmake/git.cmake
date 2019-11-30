# This is cool, but not sure it is better than the bash script

find_package(Git QUIET)

function(GIT_BUILD REPO URL)
    message(STATUS "========================================================")
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/tmp/${REPO})
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --depth 1 ${URL}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp
            RESULT_VARIABLE GIT_RESULT)
        if(NOT GIT_RESULT EQUAL "0")
            message(FATAL_ERROR "git ${REPO} failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${REPO}/build)
    execute_process(
        COMMAND cmake ..
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${REPO}/build
        RESULT_VARIABLE GIT_RESULT
    )
    message(STATUS ">> ${REPO} cmake ${GIT_RESULT}")
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${REPO}/build
        RESULT_VARIABLE GIT_RESULT
    )
    message(STATUS ">> ${REPO} make ${GIT_RESULT}")
endfunction()

if (GIT_FOUND)
    message(STATUS ">> git [${GIT_VERSION_STRING}]:${GIT_EXECUTABLE} ")
    git_build(marko "git@github.com:gecko-robotics/marko.git")
endif()
