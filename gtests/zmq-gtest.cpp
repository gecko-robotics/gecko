#include <gtest/gtest.h> // googletest header file
#include "transport.hpp"
#include <string>

using namespace std;


TEST(zmqTCP, CStrEqual) {
    EXPECT_STREQ("tcp://1.2.3.4:*", zmqTCP("1.2.3.4").c_str());
    EXPECT_STREQ("tcp://1.2.3.4:12345", zmqTCP("1.2.3.4", "12345").c_str());
}
