/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

// https://en.cppreference.com/w/cpp/chrono

/*
https://www.geeksforgeeks.org/chrono-in-c/

system_clock-It is the current time according to the system (regular clock
  which we see on the toolbar of the computer). It is written as-
  std::chrono::system_clock
steady_clock-It is a monotonic clock that will never be adjusted.It goes at a
  uniform rate. It is written as- std::chrono::steady_clock
high_resolution_clock– It provides the smallest possible tick period. It is
  written as-std::chrono::high_resolution_clock
*/

/*
https://en.cppreference.com/w/cpp/chrono/c/timespec_get
https://en.cppreference.com/w/cpp/chrono/c
C++17
If base is TIME_UTC, then:
- ts->tv_sec is set to the number of seconds since epoch, truncated to a whole value
- ts->tv_nsec is set to the integral number of nanoseconds, rounded to the resolution of the system clock
#include <cstdio>
#include <ctime>

inline double difftime(const timespec &a, const timespec &b) {
    return (double)(a.tv_sec - b.tv_sec) + 1e-9*(a.tv_nsec - b.tv_nsec);
}

int main()
{
    std::timespec ts;
    std::timespec_get(&ts, TIME_UTC);
    char buf[100];
    std::strftime(buf, sizeof buf, "%D %T", std::gmtime(&ts.tv_sec));
    std::printf("Current time: %s.%09ld UTC\n", buf, ts.tv_nsec);

    timespec ts2;
    timespec_get(&ts2, TIME_UTC);
    std::printf("diff %f\n", difftime(ts2,ts));
}
>> Current time: 06/24/16 20:07:42.949494132 UTC
*/

#include <chrono>
#include <thread>  // sleep_for
#include <string>

namespace gecko {

// double now();
// inline double from_time(double datumn){ return now() - datumn; }
inline void msleep(int msec){ std::this_thread::sleep_for(std::chrono::milliseconds(msec)); }
inline void usleep(int usec){ std::this_thread::sleep_for(std::chrono::microseconds(usec)); }
inline void  sleep(int  sec){ std::this_thread::sleep_for(std::chrono::seconds(sec)); }

}

std::string time_date();

class Clock {
public:
    Clock();
    void start();
    double stop();

protected:
    double now();
    double hack;
};


class Rate {
public:
    Rate(double hertz);
    void sleep(void);
protected:
    std::chrono::time_point<std::chrono::system_clock> last_time;
    // std::chrono::duration<double> dt;
    std::chrono::milliseconds dt;
};
