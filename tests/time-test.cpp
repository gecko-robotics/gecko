#include <iostream>
#include <stdio.h>
#include "gecko.hpp"


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

int main (){
    double start = gecko::now();
    printf(">> Start %f\n", start);
    gecko::sleep(3);

    cout << ">> sleep for 3 sec: " << gecko::from_time(start) << endl;
}
