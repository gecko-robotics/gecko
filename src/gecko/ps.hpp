#pragma once

#include <string>

class PS {
public:
    int get(int pid);

    std::string pid;
    std::string mem;
    std::string cpu;
};
