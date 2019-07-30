#pragma once

#include <exception>


// Based on python's exception
struct NotImplemented : public std::exception {
    NotImplemented(const std::string &s): msg(s) {}
    NotImplemented(): msg("Not implemented") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

struct GeckoCppError : public std::exception {
    GeckoCppError(const std::string &s): msg(s) {}
    GeckoCppError(): msg("GeckoCpp error") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};
