# - Find msgpack
# Find the native Message Pack headers and libraries.
#
#  MSGPACK_INCLUDE_DIRS - where to find msgpack.hpp, etc.
#  MSGPACK_LIBRARY      - List of libraries when using MsgPack.
#  MSGPACK_LIBRARIES    - List of libraries when using MsgPack.
#  MSGPACK_FOUND        - True if MsgPack found.


# Look for the header file.
FIND_PATH(MSGPACK_INCLUDE_DIR NAMES msgpack.hpp PATHS
    /usr/include
    /usr/local/include
    /usr/local/include/msgpack
)

# Look for the library.
FIND_LIBRARY(MSGPACK_LIBRARY NAMES msgpackc libmsgpackc PATHS
    /usr/lib
    /usr/local/lib
    /usr/local/lib/msgpack
)

SET(MSGPACK_LIBRARIES ${MSGPACK_LIBRARY})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MsgPack DEFAULT_MSG MSGPACK_LIBRARY MSGPACK_INCLUDE_DIR)
MARK_AS_ADVANCED(MSGPACK_INCLUDE_DIR MSGPACK_LIBRARY)
