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
    bool server = true;

    while((opt = getopt(argc, argv, "ha:cn:p:")) != -1)
    {
        cout << opt << endl;
        switch(opt)
        {
        case 'a':
            address = optarg;
            break;
        case 'c':
            server = false;
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
            // cout << "n**" << endl;
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
    // MCSocket ss(address, port);

    ss.bind();
    printf("====================================\n");
    printf(" Gecko Ping Test\n");
    printf(" - Address: %s:%d\n", address.c_str(), port);
    printf(" - Number pings: %d\n", how_many);
    printf(" Y: good recv  X: bad recv  .: no recv\n");
    printf("------------------------------------\n\n");

    string server_msg{"server ping"};
    string client_msg{"client ping"};
    string msg, expect;

    if(server){
        msg = server_msg;
        expect = client_msg;
    }
    else {
        msg = client_msg;
        expect = server_msg;
    }

    for (int i=0; i < how_many; i++){
        ss.cast(msg);
        gecko::msleep(500);

        // printf(">> Sent ping\n");

        string ans;
        struct sockaddr_in addr;
        tie(ans, addr) = ss.recv_nb();

        /*
        This doesn't work right now ... it keeps seeing the message
        it sends and thinks it is from the other computer ... need
        something else
        */

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            cout << t[0] << endl;
            if (t.size() > 0 && t[0] == expect){
                // printf(">> received a response: %s\n", "FIXME");
                printf("Y");
            }
            else printf("X");
        }
        else printf(".");
        fflush(stdout);  // force printf to print

        gecko::sleep(1);
    }
    printf("\n\n");

    return 0;
}
