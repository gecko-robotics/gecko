#include <stdio.h>
#include <string>
#include "color.hpp"
#include "log.hpp"

using namespace std;

int main()
{
    ColorPrint c;
    for (int i=0;i<8; i++){
        for (int j=0; j<8; j++){
            string s = c.color(i, j, s);
            printf("c++ fg:%d bg: %d %s\n", i, j, s.c_str());
        }
    }

    Logger log;
    for (int i=0; i<5; i++){
        log.loginfo("multi-part\n -> message\n -> sent\n");
        log.logwarn("multi-part\n -> message\n -> sent\n");
        log.logerror("multi-part\n -> message\n -> sent\n");
        log.logdebug("multi-part\n -> message\n -> sent\n");
    }

    return 0;
}
