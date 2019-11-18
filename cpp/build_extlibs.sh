#!/bin/bash

rm -fr tmp

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

# json ---------------------------------------------------------------
echo ">> Installing JSON in ${DIR}"
git clone https://github.com/nlohmann/json.git
mkdir -p json/build
cd json/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DJSON_BuildTests=OFF ..
make install
cd ../..

# libzmq --------------------------------------------------------------
ZMQ_VER="4.3.2"
wget https://github.com/zeromq/libzmq/releases/download/v${ZMQ_VER}/zeromq-${ZMQ_VER}.tar.gz
tar -zxvf zeromq-${ZMQ_VER}.tar.gz
mkdir -p zeromq-${ZMQ_VER}/build
cd zeromq-${ZMQ_VER}/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DWITH_DOCS=OFF -DBUILD_TESTS=OFF ..
make install
cd ../..

# cppzmq --------------------------------------------------------------
echo ">> Installing cppzmq in ${DIR}"
git clone https://github.com/zeromq/cppzmq.git
mkdir cppzmq/build
cd cppzmq/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DCPPZMQ_BUILD_TESTS=OFF ..
make install
cd ../..

# Marko ---------------------------------------------------------------
git clone https://github.com/gecko-robotics/marko.git
mkdir -p marko/build
cd marko/build
cmake -DCMAKE_INSTALL_PREFIX=${DIR} -DEXAMPLES=OFF ..
make install
cd ../..

# OpenCV --------------------------------------------------------------
echo ">> TODO: setup opencv"

cd ..
# rm -fr tmp
