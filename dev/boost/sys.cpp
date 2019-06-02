#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/process.hpp>
// #include <boost/filesystem/path.hpp>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <exception>

/*
watch -n 1 'ps -o pid,%mem,%cpu,comm -p 36879'

(.venv) kevin@dalek build $ ps -o pid,%mem,%cpu -p 17396 | sed -n '1!p'
17396  0.1  0.0
(.venv) kevin@dalek build $ ps -o pid,%mem,%cpu -p 17396 | awk '{if(NR>1)print}'17396  0.1  0.0


https://www.boost.org/doc/libs/1_64_0/doc/html/boost_process/tutorial.html
*/

using namespace std;
namespace bp = boost::process;

class PS {
public:
    int get(int pid);

    std::string pid;
    std::string mem;
    std::string cpu;
};

int PS::get(int pid){
    string line;
    bp::ipstream output;
    // boost::filesystem::path cmd = bp::search_path("ls");
    string cmd = "/bin/ps";
    bp::child p("/bin/ps -o pid,%mem,%cpu -p " + std::to_string(pid), bp::std_out > output);

    // PID %MEM %CPU
    std::getline(output, line);  // header ... don't need
    try {
        std::getline(output, line);
        // cout << "Received: " << line << "\n" << std::flush;
        stringstream ss(line);
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
    }
    catch (exception &e){
        cout << e.what() << flush;
        return 1;
    }
    catch (...){
        // cout << "wtf" << endl;
        return 1;
    }

    return 0;
}

int main(){
    PS ps;
    if (ps.get(24878) == 0)
        cout << ps.pid << " " << ps.mem << " " << ps.cpu << endl;
    else
        cout << "ERROR" << endl;


    if (ps.get(25663) == 0)
        cout << ps.pid << " " << ps.mem << " " << ps.cpu << endl;
    else
        cout << "ERROR" << endl;

    return 0;
}
