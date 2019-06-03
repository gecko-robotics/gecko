#pragma once

#include <exception>

struct NotImplemented : public std::exception {
    NotImplemented(const std::string &s): msg(s) {}
    NotImplemented(): msg("Not implemented") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

// struct InvalidKey : public std::exception {
//     InvalidKey(const std::string &s): msg(s) {}
//     InvalidKey(): msg("Invalid Key") {}
//     const char * what () const throw () {return msg.c_str();}
// protected:
//     std::string msg;
// };
