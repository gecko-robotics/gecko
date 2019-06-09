#include "gecko/color.hpp"
#include <string>
#include <sstream>


using namespace std;


string ColorPrint::color(int fg, const std::string& s){
    stringstream ss;
    // const char* fmt = (reset ? "\x1B[%d;%dm%s\x1B[0m" : "\x1B[%d;%dm%s");
    ss << "\x1B[" << to_string(fg+30) << 'm' << s << "\x1B[0m";
    // ss << "\e[4;32m" << s << "\e[0;30m";
    return ss.str();
}

string ColorPrint::color(int fg, int bg, const std::string& s){
    stringstream ss;
    // const char* fmt = (reset ? "\x1B[%d;%dm%s\x1B[0m" : "\x1B[%d;%dm%s");
    ss << "\x1B[" << to_string(fg+30) << ';' << to_string(bg+40) << 'm' << s << "\x1B[0m";
    // ss << "\e[4;32m" << s << "\e[0;30m";
    // ss << "\033[1;31m" << s << "\033[0m";
    return ss.str();
}

///////////////////////////////////////////////////////////////////////
// char* ColorPrint::color(int fg, const char* s, bool reset)
// {
//     // static char command[13];
//     /* Command is the control command to the terminal */
//     // sprintf(command, "\x1B[%d;%d;%dm%s", attr, fg + 30, bg + 40, s);
//     const char* fmt = (reset ? "\x1B[%dm%s\x1B[0m" : "\x1B[%dm%s");
//     sprintf(command, fmt, fg + 30, s);
//     return command;
// }
//
// char* ColorPrint::color(int fg, int bg, const char* s, bool reset)
// {
//     // static char command[13];
//     /* Command is the control command to the terminal */
//     // sprintf(command, "\x1B[%d;%d;%dm%s", attr, fg + 30, bg + 40, s);
//     const char* fmt = (reset ? "\x1B[%d;%dm%s\x1B[0m" : "\x1B[%d;%dm%s");
//     sprintf(command, fmt, fg + 30, bg + 40, s);
//     return command;
// }
//
// char* ColorPrint::color(int attr, int fg, int bg, const char* s, bool reset)
// {
//     // static char command[13];
//     /* Command is the control command to the terminal */
//     // sprintf(command, "\x1B[%d;%d;%dm%s", attr, fg + 30, bg + 40, s);
//     const char* fmt = (reset ? "\x1B[%d;%d;%dm%s\x1B[0m" : "\x1B[%d;%d;%dm%s");
//     sprintf(command, fmt, attr, fg + 30, bg + 40, s);
//     return command;
// }
// char* color(const& a_t t, bool reset=true)
// {
//     // static char command[13];
//     /* Command is the control command to the terminal */
//     // sprintf(command, "\x1B[%d;%d;%dm%s", attr, fg + 30, bg + 40, s);
//     // const char* fmt = (reset ? "\x1B[%d;%d;%dm%s\x1B[0m" : "\x1B[%d;%d;%dm%s");
//     // sprintf(command, fmt, attr, fg + 30, bg + 40, s);
//
//     return command;
// }
