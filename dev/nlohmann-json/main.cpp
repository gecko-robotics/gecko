#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <nlohmann/json.hpp>

// using json = nlohmann::json;
using namespace nlohmann;
using namespace std;


class pt {
public:
    pt(const double a, const double r){angle=a;range=r;}
    double angle, range;
};

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
    // vector<pt> pts = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}};
    j["pts"] = pts;
    j["bob"] = 3.45678987654321e-6;

    string sj = j.dump();
    printf(">> string:\n=====================\n%s\n", sj.c_str());

    cout << "\nFormated:\n==========================" << endl;
    cout << j.dump(2) << endl;

    std::ofstream file("test.json");
    file << j << endl;

    std::ifstream ifs("test.json");
    json fj = json::parse(ifs);

    cout << "\n\nRead from file\n===================" << endl;
    cout << fj.dump(2) << endl;

    cout << "\nAccess:\n====================" << endl;
    cout << fj["pts"] << endl;
    cout << fj["bob"] << endl;

    return 0;
}
