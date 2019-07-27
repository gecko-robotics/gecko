#pragma once

#include <ssocket.hpp>
#include <string>

class BSocket: public SSocket {
public:
    BSocket(){}

    void broadcast(const std::string& msg, int port){}
};
