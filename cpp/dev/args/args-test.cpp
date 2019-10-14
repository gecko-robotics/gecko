#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <exception>
#include <sstream>
// #include <locale>

struct InvalidArgument : public std::exception {
    InvalidArgument(const std::string &s): msg("Invalid Arg: " + s) {}
    InvalidArgument(): msg("Invalid Arg") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

using namespace std;


// class Test {
// public:
//     Test(std::string a): hi(a) {cout<<hi<<endl;}
//     std::string hi;
// };


// template<typename T>
class Value {
public:
    Value() {}
    Value(std::string def, std::string help): value(def), help_msg(help) {}
    Value(const Value& v): value(v.value), help_msg(v.help_msg) {}

    template<typename T>
    T as() {
        // try {
            T ret;
            parse(ret);
            return ret;
        // }
        // catch {
        //     cout << "wtf" <<endl;
        // }
    }

    inline void parse(std::string& v){v = value;}  // this is stupid
    inline void parse(double& v){v = stod(value);}
    inline void parse(float& v){v = stof(value);}
    inline void parse(int& v){v = stoi(value);}
    inline void parse(bool& v){
        // if (s == "true") v = true;
        // else if (s == "false") v = false;
        if (value == "false" || value == "0") v = false;
        else v = true;
        // else throw exception();
    }

    std::string help(){
        return help_msg;
    }

    std::string help_msg, value;
};

class Options {
public:
    void parse() {}
    void add_place(const std::string& group){
        std::string toks;
        std::stringstream ss(group);
        std::string token;
        try{
            while(getline(ss,token,' ')) {
                // toks.push_back(token);
                opts[token] = Value(token, "");
            }
        }
        catch(exception& e){
            cout << e.what() << endl;
            // toks.clear();
            // return toks;
            exit(1);
        }
        // return toks;
    }
    inline void add_options(std::string key, std::string help, std::string def = nullptr){
        // if (key == nullptr)
        // Value v(def, help);
        opts[key] = Value(def, help);
    }

    Value& operator[](const std::string& key) {
        std::map<std::string, Value>::iterator it;
        it = opts.find(key);
        // if (it == opts.end()) throw InvalidKey(key);
        // string pid, endpt;
        // tie(pid, endpt) = it->second;
        return it->second;
    }

    map<std::string, Value> opts;
};



int main(int argc, char** argv){
    char* fargv[] =
    {
      (char*)("./main"),
      (char*)("--type"),
      (char*)("Finishing"),
      (char*)("-b")
    };

    map<string, string> args;
    vector<string> v;

    for (int i=0; i < argc; i++) v.push_back(argv[i]);

    for (int i=0; i < v.size(); i++) {
        // cout << s << endl;
        string key = v[i];
        if (i == 0){
            cout << "Program: " << key << endl;
            continue;
        }
        if (key.find("--") == 0){
            cout << "key " << key << endl;
            i++;
            string val = v[i];
            args[key] = val;
        }
        else
            cout << "nothing " << key << endl;
    }

    cout << "Map ------------------" << endl;
    for (auto const& [key, val]: args){
        if (key == "--you")
            cout << key << ": " << val << endl;
        else
            cout << key << ": " << val << endl;
    }

    cout << "=========================" << endl;

    Options option;
    option.add_place("filename type");
    option.add_options("test","this is the help for test","-4.44");
    option.add_options("tt","this is the help for tt","true");
    cout << option["test"].as<double>() << endl;
    cout << option["tt"].as<bool>() << endl;
    // option
    //     .add_options()
    //     ("hi")
    //     ("ho")
    //     ("hello")
    //     ;

    cout << stod("3.12") << endl;

    Value val("3.12","hi");
    cout << "as double/2.0: " << val.as<double>()/2.0 << " " << typeid(val.as<double>()).name() << endl;
    cout << "as float: " << val.as<float>() << " " << typeid(val.as<float>()).name() << endl;
    cout << "as int: " << val.as<int>() << " " << typeid(val.as<int>()).name() << endl;
    cout << "as bool: " << val.as<bool>() << " " << typeid(val.as<bool>()).name() << endl;

    return 0;
}
