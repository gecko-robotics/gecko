#!/bin/bash

mkdir tmp
cd tmp

DIR="/opt/gecko"
# DIR=$(pwd)/extlibs/system

# sudo mkdir ${DIR}
# sudo chown -R ${USER} ${DIR}

# # MsgPack ----------------------------------------------------------
# echo ">> Installing MsgPack in ${DIR}"
# git clone https://github.com/msgpack/msgpack-c.git
# mkdir msgpack-c/build
# cd msgpack-c/build
# cmake -DMSGPACK_CXX[17]=ON -DCMAKE_INSTALL_PREFIX=${DIR} ..
# make install
# cd ../..
# # rm -fr msgpack-c



# json ---------------------------------------------------------------
echo ">> Installing JSON in ${DIR}"
git clone https://github.com/nlohmann/json.git
mkdir json/build
cd json/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DJSON_BuildTests=OFF ..
make install
cd ../..
# rm -fr json

# libzmq --------------------------------------------------------------
ZMQ_VER="4.3.2"
wget https://github.com/zeromq/libzmq/releases/download/v${ZMQ_VER}/zeromq-${ZMQ_VER}.tar.gz
mkdir -p zeromq-${ZMQ_VER}/build
cd zeromq-${ZMQ_VER}/build
# cmake -DCMAKE_INSTALL_PREFIX=${DIR} ..
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DWITH_DOCS=OFF -DBUILD_TESTS=OFF ..
make install
cd ../..
# rm -fr zeromq-${ZMQ_VER}

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
