#pragma once

#include <string>
#include <gecko/protobuf/msgs.pb.h>
#include <zmq.hpp>

namespace geckopb {

template <class MSG>
zmq::message_t protobufPack(MSG& m){
    std::string s;
    m.SerializeToString(&s);
    zmq::message_t msg(static_cast<void*>(s.data()), s.size());
    return std::move(msg);
}

template <class MSG>
MSG protobufUnpack(zmq::message_t& msg){
    std::string m(reinterpret_cast<const char*>(msg.data()), msg.size());
    MSG v;
    v.ParseFromString(m);
    return std::move(v);
}

}
