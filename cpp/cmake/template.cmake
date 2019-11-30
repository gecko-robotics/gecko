message(STATUS ">> Installing ${PROJECT_NAME} to: ${CMAKE_INSTALL_PREFIX}")
# use cmake to create a version header file
configure_file (
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/version.hpp.in"
    "${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/version.hpp"
)

install(
    TARGETS ${PROJECT_NAME}
    # EXPORT ${PROJECT_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    # INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  FILES_MATCHING PATTERN "*.h*"
  PATTERN "*.in" EXCLUDE
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(
    cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    @ONLY
)

install(
    FILES
        "${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Build.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}
)
