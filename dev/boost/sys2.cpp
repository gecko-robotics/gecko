#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <memory>
#include <array>

/*
watch -n 1 'ps -o pid,%mem,%cpu,comm -p 36879'

(.venv) kevin@dalek build $ ps -o pid,%mem,%cpu -p 17396 | sed -n '1!p'
17396  0.1  0.0
(.venv) kevin@dalek build $ ps -o pid,%mem,%cpu -p 17396 | awk '{if(NR>1)print}'17396  0.1  0.0


https://www.boost.org/doc/libs/1_64_0/doc/html/boost_process/tutorial.html
*/

using namespace std;

class PS {
public:
    int get(int pid);

    std::string pid;
    std::string mem;
    std::string cpu;
};

int PS::get(int pid){
    string cmd = "/bin/ps -o pid,%mem,%cpu -p " + std::to_string(pid) + " | sed -n '1!p'";

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
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

    return 0;
}

int main(){
    PS ps;
    if (ps.get(51288) == 0)
        cout << ps.pid << " " << ps.mem << " " << ps.cpu << endl;
    else
        cout << "ERROR" << endl;


    if (ps.get(25663) == 0)
        cout << ps.pid << " " << ps.mem << " " << ps.cpu << endl;
    else
        cout << "ERROR" << endl;

    return 0;
}
