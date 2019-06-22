######################################################################
# Package Generator ------------------
if(APPLE)
    set(CPACK_GENERATOR "DMG")
    # PackageMaker (Mac OSX) packaging.
    # find_program(PACKAGE_MAKER "PackageMaker")
    # if(PACKAGE_MAKER)
    #     set(CPACK_GENERATOR "PackageMaker")
    # endif()
else()
    set(CPACK_GENERATOR "DEB")
    # DEB (Debian, Ubuntu) packaging.
    find_program(DPKG "dpkg")
    if(DPKG)
        set(CPACK_GENERATOR "DEB")
        if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
        elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
        elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armhf")  # RPi
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
        endif()
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Me <support@example.com>")
        set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    endif()
    # set(CPACK_SOURCE_GENERATOR "TGZ")
endif()
SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "msgpack")
set(CPACK_PACKAGE_DESCRIPTION "test")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "test")
set(CPACK_PACKAGE_CONTACT "Kevin Walchko")
# set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.html")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
# set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.html")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

include(CPack)

# Install Gecko ----------------------------------------------------
# You can redefine at build time:
#   cmake .. -DCMAKE_INSTALL_PREFIX=/home/kevin/.local
# To create debian package, run: make package
#
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

file(GLOB HEADERS src/gecko/*.hpp)
install(FILES ${HEADERS} DESTINATION include/gecko)

# CppZmq -----------------------------------------------------------
install(
    FILES extlibs/cppzmq/zmq.hpp extlibs/cppzmq/zmq_addon.hpp
    DESTINATION include/gecko/zmq
)

# MsgPack-Gecko -----------------------------------------------------
if(BUILD_MSGPACK)
    file(GLOB HEADERS extlibs/gecko-msgpack/gecko/msgpack/*.hpp)
    install(FILES ${HEADERS} DESTINATION include/gecko/msgpack)
endif()

# Json ---------------------------------------------------------------
# file(GLOB_RECURSE HEADERS extlibs/nlohmann-json/include/*)
# install(FILES ${HEADERS} DESTINATION include/gecko/json)
install(
    DIRECTORY extlibs/nlohmann-json/include/
    DESTINATION include/gecko/json
)
