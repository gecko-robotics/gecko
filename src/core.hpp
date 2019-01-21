#pragma once

#include <map>
#include <string>
#include <vector>
#include "signals.hpp"
#include "zmq.hpp"
#include "mbeacon.hpp"

// namespace gecko {

class Directory {
public:
    void push(const std::string& key, const std::string& topic, const std::string& endpt);  // add
    void pop(const std::string& key, const std::string& topic);  // remove
    std::string find(const std::string& key, const std::string& topic);
    void print() const;
    int numberKeys() const;
    int numberTopics(const std::string& key) const;
    std::map<std::string, std::map<std::string, std::string>> db;
};

// class Core: protected SigCapture {
class Core {
    /*
    GeckoCore
    */
public:
    Core(std::string grp={"239.255.255.250"}, int port=11311);
    ~Core();
    void run(int hertz=100);
    void requestLoop(void);

    std::map<std::string, std::string> directory;
    std::string key;
    bool ok;
};
