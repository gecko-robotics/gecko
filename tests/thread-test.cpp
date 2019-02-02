#include <iostream>
#include <chrono>
#include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <exception>
#include "event.hpp"

using namespace std;

// struct NotImplemented : public exception {
//     NotImplemented(const std::string &s): msg(s) {}
//     NotImplemented(): msg("Not implemented") {}
//     const char * what () const throw () {return msg.c_str();}
// protected:
//     std::string msg;
// };


// // A version of python's Event class. Assume is_set/set of a bool is atomic and
// // no need for a mutex.
// class Event {
// public:
//     Event(): flag(false) {}
//     // Event(const Event& e): flag(e.is_set()) {}
//     Event(const Event& e) = delete;
//     inline void set() {flag = true;}
//     inline void clear() {flag = false;}
//     inline bool is_set() const {return flag;}
//     void wait(int timeout_msec=-1){
//         // throw exception("Evvent::wait not enabled");
//         throw NotImplemented("Event::wait not enabled");
//
//         // https://en.cppreference.com/w/cpp/thread/condition_variable/wait_for
//         // Wait until flag is set
//         // std::unique_lock<std::mutex> lk(m);
//         // if (timeout_msec == -1) cv.wait(lk, is_set);
//         // else cv.wait_for(lk, timeout_msec*1ms, []{return is_set();});
//         // lk.unlock();
//         // cv.notify_one();
//     }
// protected:
//     bool flag;
//     std::mutex m; // for wait()
//     std::condition_variable cv; // for wait()
// };

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
