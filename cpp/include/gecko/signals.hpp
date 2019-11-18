/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

/*
 This class captures the SIGINT signal and sets ok to false. Since ok is
 static, any class that inherits this will see the status change and
 allow it close down cleanly.
 */
class SigCapture {
public:
    SigCapture();
    static void my_handler(int s); // signal handler function
    void on();          // turn on
    void shutdown();    // turn off

// protected:
    static bool ok;     // global status on if a SIGINT has occured
    bool enabled;
};
