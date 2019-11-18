#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace nlohmann;

/***
 * Is there value added with these?
 */

inline
void writeJson(const std::string& filename, const json& j){
    std::ofstream file(filename);
    file << j << std::endl;
}

inline
void readJson(const std::string& filename, json& j){
    std::ifstream ifs(filename);
    j = json::parse(ifs);
}
