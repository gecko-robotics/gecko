cmake_minimum_required(VERSION 3.6)
project(ssdp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/usr/local/include)
include_directories(./)
link_directories(/usr/local/lib)


add_executable(mrecv mcreceive.c)
add_executable(msend mcsend.c)
