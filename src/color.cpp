// #include "gecko/color.hpp"
// #include <string>
// #include <sstream>
//
//
// using namespace std;
//
//
// string ColorPrint::color(int fg, const std::string& s){
//     stringstream ss;
//     // const char* fmt = (reset ? "\x1B[%d;%dm%s\x1B[0m" : "\x1B[%d;%dm%s");
//     ss << "\x1B[" << to_string(fg+30) << 'm' << s << "\x1B[0m";
//     // ss << "\e[4;32m" << s << "\e[0;30m";
//     return ss.str();
// }
//
// string ColorPrint::color(int fg, int bg, const std::string& s){
//     stringstream ss;
//     // const char* fmt = (reset ? "\x1B[%d;%dm%s\x1B[0m" : "\x1B[%d;%dm%s");
//     ss << "\x1B[" << to_string(fg+30) << ';' << to_string(bg+40) << 'm' << s << "\x1B[0m";
//     // ss << "\e[4;32m" << s << "\e[0;30m";
//     // ss << "\033[1;31m" << s << "\033[0m";
//     return ss.str();
// }
//
//
