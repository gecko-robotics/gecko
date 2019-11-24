#!/bin/bash

rm -fr tmp

mkdir tmp
cd tmp

DIR="/opt/gecko"

MSGPACK="false"
JSON="false"
ZMQ="false"
CPPZMQ="false"
MARKO="false"

# MsgPack ----------------------------------------------------------
if [[ ${MSGPACK} == "true" ]]; then
    echo ">> Installing MsgPack in ${DIR}"
    git clone https://github.com/msgpack/msgpack-c.git
    MP_VER="3.2.0"
    wget https://github.com/msgpack/msgpack-c/releases/download/cpp-${MP_VER}/msgpack-${MP_VER}.tar.gz
    mkdir msgpack-c/build
    cd msgpack-c/build
    cmake -DMSGPACK_CXX[17]=ON -DCMAKE_INSTALL_PREFIX=${DIR} ..
    make install
    cd ../..
fi

# json ---------------------------------------------------------------
if [[ ${JSON} == "true" ]]; then
    echo ">> Installing JSON in ${DIR}"
    # git clone https://github.com/nlohmann/json.git
    # mkdir -p json/build
    # cd json/build
    # cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DJSON_BuildTests=OFF ..
    # make install
    # cd ../..
    JSON_VER="3.7.3"
    wget https://github.com/nlohmann/json/releases/download/v${JSON_VER}/json.hpp
    mkdir -p ${DIR}/include/nlohmann
    mv json.hpp ${DIR}/include/nlohmann/
fi

# libzmq --------------------------------------------------------------
if [[ ${ZMQ} == "true" ]]; then
    echo ">> Installing libzmq in ${DIR}"
    ZMQ_VER="4.3.2"
    wget https://github.com/zeromq/libzmq/releases/download/v${ZMQ_VER}/zeromq-${ZMQ_VER}.tar.gz
    tar -zxvf zeromq-${ZMQ_VER}.tar.gz
    mkdir -p zeromq-${ZMQ_VER}/build
    cd zeromq-${ZMQ_VER}/build
    cmake -DCMAKE_INSTALL_PREFIX=${DIR} \
        -DENABLE_CPACK=OFF \
        -DWITH_DOC:BOOL=OFF \
        -DWITH_DOCS:BOOL=OFF \
        -DBUILD_STATIC:BOOL=ON \
        -DBUILD_TESTS:BOOL=OFF ..
    make install
    cd ../..
fi

# cppzmq --------------------------------------------------------------
if [[ ${CPPZMQ} == "true" ]]; then
    echo ">> Installing cppzmq in ${DIR}"
    # https://github.com/zeromq/cppzmq/archive/v4.5.0.tar.gz
    CPPZMQ_VER="4.5.0"
    # git clone https://github.com/zeromq/cppzmq.git
    wget https://github.com/zeromq/cppzmq/archive/v${CPPZMQ_VER}.tar.gz
    tar zxvf "v${CPPZMQ_VER}.tar.gz"
    cd cppzmq-*
    mkdir -p build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=${DIR} \
        -DCPPZMQ_BUILD_TESTS:BOOL=OFF ..
    make install
    cd ../..
fi

# Marko ---------------------------------------------------------------
if [[ ${MARKO} == "true" ]]; then
    git clone https://github.com/gecko-robotics/marko.git
    mkdir -p marko/build
    cd marko/build
    cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DEXAMPLES=OFF ..
    make install
    cd ../..
fi

# OpenCV --------------------------------------------------------------
if [[ ${CV} == "true" ]]; then
    echo ">> TODO: setup opencv"
fi

cd ..
# rm -fr tmp
