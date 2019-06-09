/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once


#include <string>
#include <vector>

// typedef std::vector<std::string> ascii_t;

using ascii_t = std::vector<std::string>;

class Ascii {
public:
    Ascii(char separator='|'): sep(separator) {}
    ascii_t& unpack(const std::string& str);
    std::string pack(ascii_t& v);

protected:
    ascii_t toks;
    char sep;
};
