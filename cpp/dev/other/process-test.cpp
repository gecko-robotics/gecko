// #include "node.hpp"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>  // pid (type int)
#include <csignal>  // signals

using namespace std;

void sigHandler(int signo)
{
    std::cout << "Got a signal: " << signo << std::endl;;
    exit(signo);
}

void child(){

        // child process
        int i = 0;
        pid_t pid = getpid();
        for (; i < 25; ++i)
        // while(true)
        {
            printf("child process[%d]: counter=%d\n", pid, i);
            sleep(1);
        }
}

int main(){
    printf("--beginning of program\n");

    int counter = 0;
    pid_t pid = fork();

    if (pid == 0){
        pid_t sid = setsid();
        printf("child sid: %d\n", sid);
        signal(SIGHUP, sigHandler);
        child();
    }
    else if (pid > 0)
    {
        // parent process
        int j = 0;
        pid_t pid = getpid();
        for (; j < 5; ++j)
        {
            printf("parent process[%d]: counter=%d\n", pid, j);
            sleep(1);
        }

        killpg(pid, SIGHUP);
    }
    // else
    // {
    //     // fork failed
    //     printf("fork() failed!\n");
    //     return 1;
    // }

    printf("--end of program--\n");
}
