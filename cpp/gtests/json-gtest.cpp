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
    EXPECT_TRUE(j["pts"][1] == 2);
}

TEST(json, file) {
    json j = {
        {"pi", 3.14},
        {"more", {1,2,3,4,1e-9,1e9}},
        {"other",
            {
                {"hi", "ho"},
                {"ho", "hi"},
                {"more", {3,2,1,"hehe"}}
            }
        }
    };
    json jj;
    writeJson("test.json", j);
    readJson("test.json", jj);

    EXPECT_TRUE(j == jj);
    EXPECT_TRUE(jj["pi"] == 3.14);
    EXPECT_TRUE(jj["more"][2] == 3);
    EXPECT_TRUE(jj["other"]["hi"] == "ho");
    EXPECT_TRUE(jj["other"]["more"][1] == 2);
}
