#pragma once

#include "gecko.hpp"
#include <string>

class Logger {
public:
    enum LogTypes {
        STDOUT=1,
        TCP   =2,
        UDS   =4
    };

    // Logger(int type=0);
    Logger(); // stdout
    Logger(std::string key, std::string topic, bool tcp=true); // tcp
    // Logger(std::string file); // uds

    // void print(const std::string& s) const {}
    void logerror(const std::string& s) const;
    void logwarn(const std::string& s) const;
    void loginfo(const std::string& s) const;
    void logdebug(const std::string& s) const;

protected:
    void log(const std::string& s) const; // printf
    Publisher *publisher;
    int type;
};

// class LogPub : public Logger {
// public:
//     // create publisher that will send measages to the main logger
//     LogPub(std::string& key, bool bind=false) {}
// protected:
//     void log(const std::string& s) const; // pub to server
// };


// class LogSub : public Logger {
// public:
//     // create subscriber that will receive all log measages
//     LogSub(std::string& key, bool bind=false) {}
// // protected:
// //     void log(const std::string& s) const; // printf
// };
