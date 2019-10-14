#pragma once

#include <time.h>
#include <ctime>

class Clock {
public:
    Clock();
    void start();
    void stop();
    void print();

protected:
    timespec start_time, stop_time;
};
//
// #include <stdio.h>
//
// static inline double difftime(const timespec &a, const timespec &b) {
//     return (double)(a.tv_sec - b.tv_sec) + 1e-9*(a.tv_nsec - b.tv_nsec);
// }
//
// Clock::Clock() {}
//
// void Clock::start() {
//     std::timespec_get(&start_time, TIME_UTC);
// }
//
// void Clock::stop() {
//     std::timespec_get(&stop_time, TIME_UTC);
// }
//
// void Clock::print() {
//     double dt = difftime(stop_time, start_time);
//     printf("Results:\n");
//     printf(" CPU Clock time: %d sec");
//     printf(" Wall Clock time: %d sec");
// }
