#pragma once

#include <string>

class PS {
public:
    int get(const std::string& pid);

    std::string pid;
    std::string mem;
    std::string cpu;
};
