#include <gtest/gtest.h> // googletest header file
#include "gecko/ascii.hpp"
#include <string>

using namespace std;


TEST(multicast, ascii) {
    Ascii a;
    ascii_t t = {"one","two","three"};
    string s = a.pack(t);
    EXPECT_TRUE("one|two|three" == s);

    ascii_t tt = a.unpack(s);
    for (int i=0; i<3; i++)
        EXPECT_TRUE(t[i] == tt[i]);
}

// int main(int argc, char** argv){
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
