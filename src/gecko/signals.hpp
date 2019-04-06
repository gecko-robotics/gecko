/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

class SigCapture {
public:
    SigCapture();
    static void my_handler(int s);
    // bool ok(){return ok;}
    void on(); // turn on
    void shutdown();
    // bool isOk();

// protected:
    static bool ok;
};
