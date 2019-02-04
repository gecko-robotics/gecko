#include <stdio.h>
#include <string>
#include "color.hpp"

using namespace std;

int main()
{
    ColorPrint c;

    // cout << "\033[1;31mbold red text\033[0m\n";
    // printf("%s\n", FRED("hello -> there"));
    // printf("%s\n", c.color(1,4,"hello -> there"));
    // printf("%s\n", c.color(1,1,4,"hello -> there"));
    // printf("%s\n", c.color(
    //     ColorPrint::Attributes::Underline,
    //     ColorPrint::Color::Black,
    //     ColorPrint::Color::Red,
    //     "hello -> there"));
    // printf("%s\n", c.color(
    //     ColorPrint::Color::Yellow,
    //     "hello -> there"));
    // for (int i=0;i<16; i++)
    //     printf("[%d]%s\n", i, c.color(
    //         i,
    //         ColorPrint::Color::Black,
    //         ColorPrint::Color::Red,
    //         "hello -> there"));

    // string s("hello");
    // for (int i=0;i<8; i++){
    //     string s = c.color(i, "foreground color");
    //     printf("c++ %d %s\n", i, s.c_str());
    // }
    string s("color test ... :)");
    for (int i=0;i<8; i++){
        for (int j=0; j<8; j++){
            string s = c.color(i, j, s);
            printf("c++ fg:%d bg: %d %s\n", i, j, s.c_str());
        }
    }

    return 0;
}
