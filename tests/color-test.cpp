#include <stdio.h>
#include "color.hpp"


int main()
{
    ColorPrint c;

    // cout << "\033[1;31mbold red text\033[0m\n";
    // printf("%s\n", FRED("hello -> there"));
    printf("%s\n", c.color(1,4,"hello -> there"));
    printf("%s\n", c.color(1,1,4,"hello -> there"));
    printf("%s\n", c.color(
        ColorPrint::Attributes::Underline,
        ColorPrint::Color::Black,
        ColorPrint::Color::Red,
        "hello -> there"));
    printf("%s\n", c.color(
        ColorPrint::Color::Yellow,
        "hello -> there"));
    for (int i=0;i<16; i++)
        printf("[%d]%s\n", i, c.color(
            i,
            ColorPrint::Color::Black,
            ColorPrint::Color::Red,
            "hello -> there"));

    return 0;
}
