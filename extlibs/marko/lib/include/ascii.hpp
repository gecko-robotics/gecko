/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once


#include <string>
#include <vector>


using ascii_t = std::vector<std::string>;

/*
 The multicast beacon uses a simple ASCII encoding for message traffic. why?
 The messages tend to be small and you waste more time encoding/decoding
 when you use something fancier. Also, SDP uses ASCII and that works.

 Ex: (key|topic|pid) -> dalek|camera|13412

 */
class Ascii {
public:
    Ascii(char separator='|'): sep(separator) {}
    ascii_t& unpack(const std::string& str); // network to host
    std::string pack(ascii_t& v);            // host to network

protected:
    ascii_t toks;
    char sep;
};
