/********************************************\
 * The MIT License (MIT)
 * Copyright (c) 2014 Kevin Walchko
 * see LICENSE for full details
\********************************************/
#include "gecko/gecko.hpp"
// #include <boost/asio.hpp>
#include <iostream>
#include <string>
// #include <map>
// #include <unistd.h>     // getpid
// #include <stdlib.h>
#include <exception>
#include <stdio.h>
#include <unistd.h>

/*
========================================
 Geckocore [65975]
-------------
 Key: local
 Host IP: 10.0.1.57
 Listening on: 224.3.29.110:11311
-------------
Known Services [6]
 * hello:........................ tcp://10.0.1.57:65303
 * hey there:.................... tcp://10.0.1.57:65304
 * ryan:......................... tcp://10.0.1.57:65310
 * mike:......................... tcp://10.0.1.57:65311
 * sammie:....................... tcp://10.0.1.57:65312
 * scott:........................ tcp://10.0.1.57:65313

Binders [6]
 [65993] hello................. cpu:  0.0%  mem:  0.0%
 [65994] hey there............. cpu:  0.0%  mem:  0.0%
 [66008] ryan.................. cpu:  0.1%  mem:  0.0%
 [66010] mike.................. cpu:  0.1%  mem:  0.0%
 [66012] sammie................ cpu:  0.1%  mem:  0.0%
 [66014] scott................. cpu:  0.1%  mem:  0.0%

Connections [8]
 [65995] hello................. cpu: 20.7%  mem:  0.0%
 [65996] hello................. cpu: 20.9%  mem:  0.0%
 [65997] hey there............. cpu: 21.0%  mem:  0.0%
 [65998] hey there............. cpu: 20.8%  mem:  0.0%
 [66011] mike.................. cpu: 19.0%  mem:  0.0%
 [66013] sammie................ cpu: 19.0%  mem:  0.0%
 [66015] scott................. cpu: 19.4%  mem:  0.0%
 [66009] ryan.................. cpu: 18.7%  mem:  0.0%


 change
 Binders [6]
  [PID]   Topic                  CPU   MEM   EndPoint
  [65993] hello................. 0.0%  0.0%  tcp://10.0.1.57:65303
  [65994] hey there............. 0.0%  0.0%  tcp://10.0.1.57:65303
  [66008] ryan.................. 0.1%  0.0%  uds:///var/run/ryan-0
*/


using namespace std;

int main(int argc, char *argv[]){
    string key;
    int opt;
    int port = gecko::mc_port;
    string addr = gecko::mc_addr;
    bool verbose = true;

    // put ':' in the starting of the
    // string so that program can
    // distinguish between '?' and ':'
    while((opt = getopt(argc, argv, ":k:hm:vp:")) != -1)
    {
        switch(opt)
        {
        case 'h':
            printf("\n%s \n", argv[0]);
            printf("--------------------------------------------\n");
            printf("GeckoCore is the connection manager for gecko. It uses\n");
            printf("a UDP broadcast to find pub/sub and make connections.\n");
            printf("following options are available:\n");
            printf("\n");
            printf("h .......... Print this help message\n");
            printf("k [key] .... Key to find the core, default hostname\n");
            printf("m [addr] ... Multicast address, default %s\n", addr.c_str());
            printf("p [port] ... Port number for broadcast, default %d\n", port);
            printf("v .......... Turn on verbose printing, default is %s\n", verbose ? "true" : "false");
            printf("\n");
            return 0;
            break;
        case 'k':
            // printf("key: %s\n", optarg);
            key = optarg;
            break;
        case 'm':
            // printf("key: %s\n", optarg);
            addr = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'v':
            verbose = true;
            break;
        case ':':
            printf("option needs a value\n");
            break;
        case '?':
            printf("unknown arguments: %c\n", optopt);
            break;
        }
    }

    // optind is for the extra arguments
    // which are not parsed
    bool bye = false;
    for(; optind < argc; optind++){
        printf("unknown arguments: %s\n", argv[optind]);
        bye = true;
    }
    if (bye) return 1;

    if (key.size() == 0){
        HostInfo host;
        key = host.cleanHostname();
    }

    BeaconCoreServer core(key);
    core.listen(verbose);  // crashes??

    return 0;
}
