#include <gtest/gtest.h> // googletest header file
#include "gecko/gecko.hpp"
#include <string>
#include <iostream>

using namespace std;

TEST(db, keys) {
    DBv db;

    db.pushbind("a", "1234","tcp://hi:123");
    db.pushbind("b", "1234","uds:///var/run/b-0");

    EXPECT_TRUE(db.get("a") == "tcp://hi:123");
    EXPECT_TRUE(db.get("b") == "uds:///var/run/b-0");
    EXPECT_THROW(db.get("c"), InvalidKey);
}
