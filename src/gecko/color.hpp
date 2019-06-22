/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once


#include <string>


namespace ColorPrint {

enum Color
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Default = 9
};

inline
std::string color(int fg, const std::string& s){
    return "\x1B[" + std::to_string(fg+30) + "m" + s + "\x1B[0m";
}

inline
std::string color(int fg, int bg, const std::string& s){
    return "\x1B[" + std::to_string(fg+30) + ";" + std::to_string(bg+40) + "m" + s + "\x1B[0m";
}
}
