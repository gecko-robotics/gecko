#pragma once

// #include <iostream>
// #include <sstream>
// #include <thread>
// #include <mutex>
#include <string>
// #include <stdio.h>
// #include "color.hpp"

class Logger {
public:
    // void print(const std::string& s) const {}
    void logerror(const std::string& s) const;
    void logwarn(const std::string& s) const;
    void loginfo(const std::string& s) const;
    void logdebug(const std::string& s) const;

protected:
    void log(const std::string& s) const; // printf
};

class LogPub : public Logger {
public:
    // create publisher that will send measages to the main logger
    LogPub(std::string& key, bool bind=false) {}
protected:
    void log(const std::string& s) const; // pub to server
};


class LogSub : public Logger {
public:
    // create subscriber that will receive all log measages
    LogSub(std::string& key, bool bind=false) {}
// protected:
//     void log(const std::string& s) const; // printf
};
