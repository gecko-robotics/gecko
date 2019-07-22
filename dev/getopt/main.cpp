#include <stdio.h>
#include <unistd.h>
// #include <iostream>
// #include <map>

using namespace std;


int main(int argc, char *argv[])
{
    int opt;

    // put ':' in the starting of the
    // string so that program can
    // distinguish between '?' and ':'
    while((opt = getopt(argc, argv, ":if:l:rxh")) != -1)
    {
        switch(opt)
        {
        case 'h':
            printf("%s \n", argv[0]);
            printf("Simple command line test program using getopt()\n");
            printf("f [filename] File name to open\n");
            printf("i            Dummy parameter\n");
            printf("l [param]    List the parameters available\n");
            printf("r            Dummy parameter\n\n");
            break;
        case 'l':
            printf("list: %s\n", optarg);
            break;
        case 'i':
        case 'r':
                printf("option: %c\n", opt);
                break;
            case 'f':
                printf("filename: %s\n", optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    // optind is for the extra arguments
    // which are not parsed
    bool bye = false;
    for(; optind < argc; optind++){
        printf("extra arguments: %s\n", argv[optind]);
        bye = true;
    }
    if (bye) return 1;

    return 0;
}
