#include "log.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "color.hpp"

using namespace std;


// Logg::Logger(){}


void Logger::logerror(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::White, ColorPrint::Color::Red, "* ERROR:") + " " + s);
}


void Logger::logwarn(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::Black, ColorPrint::Color::Yellow, ">> WARN:") + " " + s);
}

void Logger::loginfo(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::Green, ">> ") + s);
}


void Logger::logdebug(const std::string& s) const {
    ColorPrint cp;
    log(cp.color(ColorPrint::Color::Black, ColorPrint::Color::Cyan, "[DEBUG]:") + " " + s);
}


void Logger::log(const std::string& s) const {
    printf("%s", s.c_str());
}


void LogPub::log(const std::string& s) const {
    printf("logpub\n");
    printf("%s", s.c_str());
}
