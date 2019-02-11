#include <gtest/gtest.h> // googletest header file
#include "ascii.hpp"
#include <string>

using namespace std;


TEST(ascii, CStrEqualCpp) {
    Ascii a;
    ascii_t t = {"one","two","three"};
    string s = a.pack(t);
    EXPECT_STREQ("one|two|three", s.c_str());
    ascii_t tt = a.unpack(s);
    for (int i=0; i<3; i++)
        EXPECT_STREQ(t[i].c_str(), tt[i].c_str());
}
