
#include "gecko/time.hpp"

// #include <ctime>
#include <thread>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std::chrono;
using namespace std;


string time_date(){
    time_t t = std::time(nullptr);
    struct tm lt = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&lt, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}

/////////////////////////////////////////////////////////////////////
Clock::Clock(): hack(0.0) {}

void Clock::start(){
    hack = now();
}

double Clock::stop(){
    return now() - hack;
}

double Clock::now(){
    auto now_ms = time_point_cast<microseconds>(system_clock::now());
    auto epic = now_ms.time_since_epoch();
    double time = (double)epic.count();
    return time/1E6;
}

//////////////////////////////////////////////////////////////////////

Rate::Rate(double hertz){
    last_time = time_point_cast<milliseconds>(system_clock::now());
    // dt = std::chrono::milliseconds(int(1000/hertz));
    dt = milliseconds(int(1000/hertz));
}

void Rate::sleep(void){
    auto now = time_point_cast<milliseconds>(system_clock::now());

    auto diff = duration_cast<std::chrono::milliseconds>(now - last_time);
    if (diff < dt){
        this_thread::sleep_for(dt - diff);
    }
    last_time = time_point_cast<milliseconds>(system_clock::now());
}

/////////////////////////////////////////////////////////////////////////
// long Time::now(){
//     auto now_ms = time_point_cast<milliseconds>(system_clock::now());
//     auto epic = now_ms.time_since_epoch();
//     long time = epic.count();
//     return time;
// }

// double gecko::now(){
//     auto now_ms = time_point_cast<microseconds>(system_clock::now());
//     auto epic = now_ms.time_since_epoch();
//     double time = (double)epic.count();
//     return time/1E6;
// }

// double Time::now(){
//     auto now_ms = time_point_cast<microseconds>(system_clock::now());
//     auto epic = now_ms.time_since_epoch();
//     double time = (double)epic.count();
//     return time/1E6;
// }
//
// double Time::get_time(){
//     return now() - epic;
// }

// double Time::from_time(double datumn){
//     return now() - datumn;
// }

// void Time::msleep(int msec){
//     this_thread::sleep_for(chrono::milliseconds(msec));
// }
//
// void Time::usleep(int usec){
//     this_thread::sleep_for(chrono::microseconds(usec));
// }
//
// void Time::sleep(int sec){
//     this_thread::sleep_for(chrono::seconds(sec));
// }
