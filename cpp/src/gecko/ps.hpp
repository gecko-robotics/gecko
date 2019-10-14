#pragma once

#include <string>


/*
 Simple class that uses the command line and grabs the cpu and memory useage
 of a process given a pid.

 WARNING: this is designed only to work on unix/linux because it uses the
 ps command which isn't available on windows.
 */
class PS {
public:
    int get(const std::string& pid);

    // since everything always gets turned into strings, there is
    // reason to convert between numbers and strings.
    std::string pid;
    std::string mem;
    std::string cpu;
};
