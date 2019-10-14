// #include "node.hpp"
#include <stdio.h>
#include <iostream>
#include <unistd.h>     // sleep
#include <sys/types.h>  // pid (type int)
#include <csignal>      // signals
#include <vector>

using namespace std;

void sigHandler(int signo)
{
    printf(">> Process[%d] got signal: %d\n", getpid(), signo);
    exit(0);
}

void child(){

        // child process
        pid_t pid = getpid();
        for (int i=0; i < 15; ++i)
        {
            printf(">> Child process[%d]: counter=%d\n", pid, i);
            sleep(1);
        }
}

int main(){
    printf("*** Beginning of Parent Program ***\n");
    vector<pid_t> pids;  // keep track of child pids

    // spawn children
    for (int i=5; i>0; i--){
        pid_t child_pid = fork();
        printf(">> fork pid: %d\n", child_pid);

        // child -> run, then exit
        if (child_pid == 0){
            if (signal(SIGUSR1, sigHandler) == SIG_ERR)
                perror("couldn't set signal handler");
            child();
            return 0;
        }
        // parent -> save child id for later
        else if (child_pid > 0)
            pids.push_back(child_pid);
    }

    // parent only from this point on
    pid_t pid = getpid();
    for (int i=0; i < 5; i++){
        printf(">> parent process[%d]: counter=%d\n", pid, i);
        sleep(1);
    }

    for (const pid_t& p: pids) {
        int err = kill(p, SIGUSR1);
        printf(">> kill child[%d]  error: %d\n", p, err);
    }

    printf("*** End of Parent Program ***\n");
}
