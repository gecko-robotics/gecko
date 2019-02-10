#pragma once

// #include <stdio.h>
#include <string>
// #include <unistd.h>
// #include <sstream>
#include <vector>

typedef std::vector<std::string> ascii_t;

class Ascii {
public:
    ascii_t& unpack(const std::string& str);
    std::string pack(ascii_t& v, char sep='|');

protected:
    ascii_t toks;
};
