#include <gtest/gtest.h> // googletest header file
#include <gecko/gecko.hpp>
#include <string>
#include <iostream>
#include <thread>


using namespace std;

TEST(gecko, init) {
    gecko::init();
    gecko::set_broadcast(9999);
    EXPECT_TRUE(gecko::get_broadcast() == 9999);

    gecko::hello();
}


int* a=nullptr;
int* b=nullptr;

void simple(int i){
    gecko::init();

    if (i == 1) a = gecko::getAddress();
    else b = gecko::getAddress();

    gecko::sleep(1);
}

TEST(gecko, singleton) {
    // gecko::init();
    // EXPECT_TRUE(gecko::get_broadcast() == 11311);

    std::thread t1(simple, 1);
    std::thread t2(simple, 2);

    // need to wait for the threads to do their job
    while (a == nullptr || b == nullptr) gecko::msleep(100);

    cout << "Addresses: " << a << "  " << b << endl;

    // make sure they are set to something other than nullptr
    EXPECT_FALSE(a == nullptr);
    EXPECT_FALSE(b == nullptr);
    // Singleton should have the SAME memory address
    EXPECT_TRUE(a == b);

    t1.join();
    t2.join();
}
