#pragma once

#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <string>

class Logger {
public:
    Logger();
    void print(const std::string& s) const {}
}
