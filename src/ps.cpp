#include <gecko/ps.hpp>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <array>
#include <memory>


using namespace std;


int PS::get(int pid){

// I don't support windows, but I threw this in for fun
#if defined(_WIN32)
    this->pid = "0";
    this->mem = "0";
    this->cpu = "0";

#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
    string cmd = "/bin/ps -o pid,%mem,%cpu -p " + to_string(pid) + " | sed -n '1!p'";

    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe) {
        throw runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    stringstream ss(result);
    string token;
    char sep = ' ';
    vector<string> toks;
    while(getline(ss,token,sep)) {
        if (token.size() > 0) toks.push_back(token);
    }

    if (toks.size() < 3) return 1;

    this->pid = toks[0];
    this->mem = toks[1];
    this->cpu = toks[2];
#endif

    return 0;
}
