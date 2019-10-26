#!/bin/bash

mkdir -p ./python/pygecko/proto
# protoc -I=../proto --python_out=./proto ../proto/*.proto
# protoc --proto_path=. --python_out=python/pygecko pygecko/proto/*.proto
# protoc --proto_path=. --cpp_out=build proto/*.proto
# touch ./proto/__init__.py
# protoc --proto_path=extlibs/protobuf --cpp_out=extlibs/protobuf extlibs/protobuf/*.proto
protoc --proto_path=. --python_out=python/pygecko single/msgs.proto
