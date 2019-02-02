#pragma once


// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
//          foreground background
// black        30         40
// red          31         41
// green        32         42
// yellow       33         43
// blue         34         44
// magenta      35         45
// cyan         36         46
// white        37         47
//
// FOREGROUND Keys
#define RST   "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

// const char* cKRED = "\x1B[31;47m";
// const char* cRST = "\x1B[0m";
// inline const char* FRED(const char* s) {return ("\x1B[31;47m" s "\x1B[0m");}

// #define color(f,b,x) "\x1B[" atoi(f) ";" atoi(b) "m" x RST

class ColorPrint {
public:
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

    enum Attributes
    {
        Reset,  // normal
        Bold,
        Dim,
        Italics,
        Underline,
        Blink,
        Unknown0,
        Inverse,
        Hidden
        // Strikethrough = 9
    };

    // typedef struct {
    //     int attr, fg, bg;
    // } a_t;

    // I am messing up the command buffer .. writing too much ... only 13
    char* color(int fg, const char* s, bool reset=true);
    char* color(int fg, int bg, const char* s, bool reset=true);
    char* color(int attr, int fg, int bg, const char* s, bool reset=true);

protected:
    char command[13];
};
