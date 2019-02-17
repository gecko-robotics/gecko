#include <gtest/gtest.h> // googletest header file
#include "transport.hpp"
#include <string>

using namespace std;


TEST(zmq, endpoint) {
    EXPECT_TRUE("tcp://1.2.3.4:*" == zzmqTCP("1.2.3.4").endpoint);
    EXPECT_TRUE("ipc://file" == zzmqUDS("file").endpoint);
}
