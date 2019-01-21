#pragma once

#include <map>
#include <string>
#include <vector>
#include "signals.hpp"
#include "zmq.hpp"
#include "mbeacon.hpp"

// namespace gecko {

class Core: protected SigCapture {
    /*
    GeckoCore
    */
public:
    Core(std::string grp={"239.255.255.250"}, int port=11311);
    void run(int hertz=100);
    void requestLoop(void);
    
    std::map<std::string, std::string> directory;
    std::string key;
};
