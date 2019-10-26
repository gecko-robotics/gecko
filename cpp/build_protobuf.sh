#!/bin/sh bash

protoc --proto_path=extlibs/protobuf --cpp_out=extlibs/protobuf extlibs/protobuf/*.proto
