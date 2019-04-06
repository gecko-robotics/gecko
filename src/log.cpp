#include "gecko/log.hpp"
#include "gecko/color.hpp"
#include <string>
// #include <vector>
// #include <sstream>
#include <iostream>
// #include <map>

using namespace std;


// Logg::Logger(){}

Logger::Logger(): publisher(nullptr){
    type = Logger::STDOUT;
}

Logger::Logger(std::string key, std::string topic, bool tcp){
    if (tcp){
        type = Logger::TCP;
        publisher = gecko::pubConnectTCP(key, topic);
    }
    else {
        type = Logger::UDS;
        publisher = gecko::pubConnectUDS(key, topic);
    }

    if (publisher == nullptr){
        ColorPrint cp;
        string msg = cp.color(
            ColorPrint::Color::Red,
            "** Logger::Logger couldn't connect to " + key + " " + topic
        );
        cout << msg << endl;
    }
}

// Logger::Logger(std::string file){
//     type = Logger::UDS;
//     publisher = gecko::pubConnectUDS(key, topic);
//     if (publisher == nullptr){
//         ColorPrint cp;
//         cout << cp.color("** Logger::Logger couldn't connect to " + file) << endl;
//     }
// }


void Logger::logerror(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::White, ColorPrint::Color::Red, "* ERROR:") + " " + s);
}


void Logger::logwarn(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::Black, ColorPrint::Color::Yellow, ">> WARN:") + " " + s);
}

void Logger::loginfo(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::Green, ">> ") + s);
}


void Logger::logdebug(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::Black, ColorPrint::Color::Cyan, "[DEBUG]:") + " " + s);
}


void Logger::log(const std::string& s) const {
    if (type == Logger::STDOUT) printf("%s", s.c_str());
    // if (type & Logger::TCP){
    //     zmq::message_t msg((void*)s.c_str(), s.size());
    //     publisher->pub(msg);
    // }
    // else if (type & Logger::UDS) {
    //     publisher->pub(s);
    // }
    else if (type == Logger::TCP || type == Logger::UDS){
        zmq::message_t msg((void*)s.c_str(), s.size());
        publisher->publish(msg);
    }
}


// void LogPub::log(const std::string& s) const {
//     printf("logpub\n");
//     printf("%s", s.c_str());
// }
