# if (NOT APPLE)
#     set(msgpack_DIR $ENV{HOME}/.local/lib/cmake/msgpack)
#     include_directories($ENV{HOME}/.local/include)
#     link_directories($ENV{HOME}/.local/lib)
# else()
#     # include_directories($ENV{HOME}/github/gecko/src)
#     # include_directories($ENV{HOME}/github/gecko/zmq)
#     # link_directories($ENV{HOME}/github/gecko/build)
#     include_directories(/usr/local/include)
#     link_directories(/usr/local/lib)
# endif()
#
# # Get msgpack -----------------------------------------------------
# if(APPLE)
#   pkg_check_modules(MessagePack REQUIRED msgpack)
#   if(MessagePack_FOUND)
#       include_directories(${MessagePack_INCLUDE_DIR})
#       message(STATUS ">> MsgPack Found: ${MessagePack_VERSION}")
#       message(STATUS ">> MsgPack: ${MessagePack_LIBRARIES}  ${MessagePack_INCLUDE_DIR}")
#       list(APPEND ALL_LIBS ${ALL_LIBS} ${MessagePack_LIBRARIES})
#
#       # this is a template library, so no cpp
#       # include_directories(msgpack)
#       include_directories(.)
#
#   else()
#       message(WARNING "\n<<--- Could not find MsgPack --->>\n")
#   endif()
# elseif(UNIX)
#     find_package(msgpack REQUIRED)
#     if(msgpack_FOUND)
#         include_directories(${msgpack_INCLUDE_DIR})
#         message(STATUS ">> MsgPack Found: ${msgpack_VERSION}")
#         message(STATUS ">> MsgPack: ${msgpack_LIBS}  ${msgpack_INCLUDE_DIR}")
#         list(APPEND ALL_LIBS ${msgpack_LIBRARIES})
#
#         # this is a template library, so no cpp
#         # include_directories(msgpack)
#         include_directories(${CMAKE_CURRENT_LIST_DIR}/../.)
#
#     else()
#         message(WARNING "\n<<--- Could not find MsgPack --->>\n")
#     endif()
# endif()
#
# message(STATUS ">> Compiling MsgPack")
# # message(STATUS "oo> ${PATH}")
# include_directories(${CMAKE_CURRENT_LIST_DIR}/../extlibs/gecko-msgpack)
# # add_subdirectory(${PATH}/extlibs/gecko-msgpack)
