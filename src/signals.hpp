#pragma once


class SigCapture {
public:
    SigCapture();
    static void my_handler(int s);
    // bool ok(){return ok;}
    void on(); // turn on
    void shutdown();
    bool isOk();

// protected:
    static bool ok;
};
