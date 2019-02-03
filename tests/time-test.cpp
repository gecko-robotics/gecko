#include <iostream>
// #include <chrono>
// #include <ctime>
#include <thread>
#include "time.hpp"

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


// using namespace std::chrono;
using namespace std;

// class Time {
// public:
//     long now();
// };

// long Time::now(){
//     // auto now = system_clock::now();
//     auto now_ms = time_point_cast<milliseconds>(system_clock::now());
//     auto epic = now_ms.time_since_epoch();
//     long time = epic.count();
//     return time;
// }


int main (){
    Rate r(5);

    Time a;
    long s, f;
    // cout << a.now() << endl;
    s = a.now();

    // usleep(1E6);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    f = a.now();

    cout << ">> sleep for 1000: " << f-s << endl;
}
