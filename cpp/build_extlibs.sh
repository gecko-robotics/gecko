#!/bin/bash

mkdir tmp
cd tmp

DIR="/opt/gecko"
# DIR=$(pwd)/extlibs/system

# sudo mkdir ${DIR}
# sudo chown -R ${USER} ${DIR}

# MsgPack ----------------------------------------------------------
echo ">> Installing MsgPack in ${DIR}"
git clone https://github.com/msgpack/msgpack-c.git
mkdir msgpack-c/build
cd msgpack-c/build
cmake -DMSGPACK_CXX[17]=ON -DCMAKE_INSTALL_PREFIX=${DIR} ..
make install
cd ../..
# rm -fr msgpack-c

# json ---------------------------------------------------------------
echo ">> Installing JSON in ${DIR}"
git clone https://github.com/nlohmann/json.git
mkdir json/build
cd json/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DJSON_BuildTests=OFF ..
make install
cd ../..
# rm -fr json

# cppzmq --------------------------------------------------------------
echo ">> Installing cppzmq in ${DIR}"
git clone https://github.com/zeromq/cppzmq.git
mkdir cppzmq/build
cd cppzmq/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DCPPZMQ_BUILD_TESTS=OFF ..
make install
cd ../..
# rm -fr cppzmq

# OpenCV --------------------------------------------------------------
echo ">> TODO: setup opencv"

cd ..
# rm -fr tmp
