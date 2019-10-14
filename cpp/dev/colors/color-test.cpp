#include <stdio.h>
#include <string>
// #include <sstream>
#include <iostream>


using namespace std;


namespace ColorPrint {

enum Color
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Default = 9
};

inline
string color(int fg, const std::string& s){
    return "\x1B[" + to_string(fg+30) + "m" + s + "\x1B[0m";
}

inline
string color(int fg, int bg, const std::string& s){
    return "\x1B[" + to_string(fg+30) + ";" + to_string(bg+40) + "m" + s + "\x1B[0m";
}
}


namespace cp = ColorPrint;

int main()
{
    string h = "hello";
    for (int i=0;i<8; i++){
        for (int j=0; j<8; j++){
            string s = cp::color(i, j, h);
            printf("c++ fg:%d bg: %d %s\n", i, j, s.c_str());
        }
    }

    cout << cp::color(ColorPrint::Red, "DONE") << endl;

    return 0;
}
