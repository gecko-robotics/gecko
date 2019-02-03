#include <iostream>
#include <signal.h>
#include <zmq.hpp>
#include <thread>
#include "signals.hpp"
#include "event.hpp"

// static int s_interrupted = 0;
// static void s_signal_handler (int signal_value)
// {
//     s_interrupted = 1;
// }
//
// static void s_catch_signals (void)
// {
//     struct sigaction action;
//     action.sa_handler = s_signal_handler;
//     action.sa_flags = 0;
//     sigemptyset (&action.sa_mask);
//     sigaction (SIGINT, &action, NULL);
//     sigaction (SIGTERM, &action, NULL);
// }

using namespace std;


SigCapture s;

void run (void)
{
    // SigCapture s;

    cout << "Start " << s.ok << endl;

    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_SUB);
    socket.bind("tcp://*:5555");

    // while ( e.is_set() ) {
    while ( s.ok ) {
        //  Blocking read will throw on a signal
        // NO NO: cpp11 thread doesn't get the signal, so nothing is thrown
        // and this keeps blocks
        zmq::message_t msg;
        try {
            socket.recv (&msg);
        }
        catch(zmq::error_t& e) {
            // std::cout << "W: interrupt received, proceeding…" << std::endl;
            cout << e.what() << endl;
        }

    }

    cout << "Exiting now ..." << endl;
}

int main(){
    Event e;
    e.set();

    thread a(run);

    while(s.ok){}
    // run();

    cout << "main exiting" << endl;

    a.join();
}
