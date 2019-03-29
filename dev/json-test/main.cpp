// Example with Gecko
// make sure to run a pygeckocore.py instance with key: dalek.
// MIT License Kevin J Walchko 2019
//

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;


int main(){

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
    j["pts"] = pts;
    j["bob"] = 3.456;

    string sj = j.dump();
    printf(">> %s\n", sj.c_str());

    return 0;
}
