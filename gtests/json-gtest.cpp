#include <gtest/gtest.h> // googletest header file
#include "gecko/gecko.hpp"
#include <string>
#include <iostream>

using namespace std;

TEST(json, structure) {
    json j = {
      {"pi", 3.141},
      {"happy", true},
      {"name", "Niels"},
      {"nothing", nullptr},
      {"answer", {
        {"everything", 42}
      }},
      {"list", {1, 0, 2}},
      {"object", {
        {"currency", "USD"},
        {"value", 42.99}
      }}
    };

    vector<double> pts = {1,2,3,4.5,6.789};
    // vector<pt> pts = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}};
    j["pts"] = pts;
    j["bob"] = 3.45678987654321e-6;

    EXPECT_TRUE(j["bob"] == 3.45678987654321e-6);
    EXPECT_TRUE(j["pts"] == pts);
}

TEST(json, file) {
    json j = {"pi", 3.14};
    json jj;
    writeJson("test.json", j);
    readJson("test.json", jj);

    EXPECT_TRUE(j == jj);
}
