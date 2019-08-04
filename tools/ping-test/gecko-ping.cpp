/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#include <gecko/gecko.hpp>
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char *argv[]){
    int how_many = 5;
    int port = GECKOCORE_PORT;
    string address{GECKOCORE_ADDRESS};
    Ascii a;
    int opt;

    while((opt = getopt(argc, argv, "ha:p:")) != -1)
    {
        switch(opt)
        {
        case 'a':
            address = optarg;
            break;
        case 'h':
            printf("\n%s \n", argv[0]);
            printf("--------------------------------------------\n");
            printf("Gecko-Ping gets the state of all geckocores on the local\n");
            printf("network.\n");
            printf("\n");
            printf("a [addr] ... Multicast address, default %s\n", address.c_str());
            printf("h .......... Print this help message\n");
            printf("n [num] .... How many pings to send geckocores on network, default: %d\n", how_many);
            printf("p [port] ... Port number for broadcast, default %d\n", port);
            printf("\n");
            return 0;
            break;
        case 'n':
            how_many = atoi(optarg);
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case ':':
            printf("option needs a value\n");
            break;
        case '?':
            printf("unknown arguments: %c\n", optopt);
            break;
        }
    }

    BCSocket ss(port);
    ss.bind();

    for (int i=0; i < how_many; i++){
        ss.cast("geckoping");
        printf(">> Sent ping\n");

        string ans;
        struct sockaddr_in addr;
        tie(ans, addr) = ss.recv_nb();

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if (t.size() > 1 && t[0] == "geckoping"){
                printf(">> received a response: %s\n", "FIXME");
            }
            else printf(".\n");
        }
        else printf(".\n");

        gecko::sleep(1);
    }

    return 0;
}
