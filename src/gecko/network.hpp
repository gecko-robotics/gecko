/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

/*
https://beej.us/guide/bgnet/html/multi/gethostbynameman.html
PLEASE NOTE: these two functions are superseded by getaddrinfo() and
getnameinfo()! In particular, gethostbyname() doesn't work well with IPv6.
*/
#include <string>
#include <stdio.h>
#include <exception>

struct HostnameError : public std::exception {
    HostnameError(const std::string &s): msg("Couldn't determine hostname: " + s) {}
    HostnameError(): msg("Couldn't determine hostname") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

/*
 Simple class that determines a host name and ip address
 */
class HostInfo {
public:
    HostInfo();
    std::string cleanHostname(); // removes .local
    std::string hostname;
    std::string address;
};
