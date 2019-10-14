#include <iostream>
#include <chrono>
#include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <exception>
#include "event.hpp"

using namespace std;

void independentThread(Event* e, char c, int delay)
{
    std::cout << "\nStarting concurrent thread.\n";
    while(e->is_set()){
        this_thread::sleep_for(std::chrono::seconds(delay));
        cout << c << flush;
    }
    std::cout << "\nExiting concurrent thread.\n";
}

int main()
{
    Event e;
    e.set();  // flag == true

    std::thread a(independentThread, &e, '2', 2);
    a.detach();

    std::thread b(independentThread, &e, '1', 1);
    b.detach();

    // threadCaller(e);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    e.clear();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    printf("main ... bye\n");
}
