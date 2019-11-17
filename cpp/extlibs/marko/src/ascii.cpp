#include <marko/ascii.hpp>
#include <sstream>
#include <iostream>

using namespace std;

ascii_t& Ascii::unpack(const string& str){
    toks.clear();
    stringstream ss(str);
    string token;
    try{
        while(getline(ss,token,sep)) toks.push_back(token);
    }
    catch(exception& e){
        cout << e.what() << endl;
        toks.clear();
        return toks;
    }
    return toks;
}

string Ascii::pack(ascii_t& v){
    stringstream ss;
    ss << v[0];
    for (int i=1; i<v.size(); i++) ss << sep << v[i];
    return ss.str();
}
