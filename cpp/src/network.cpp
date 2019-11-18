#include <gecko/network.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

HostInfo::HostInfo(){
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;

    // To retrieve hostname
    int err = gethostname(hostbuffer, sizeof(hostbuffer));
    if (err == -1) throw HostnameError(); //cout << "hostname error" << endl;
    hostname = hostbuffer;

    // see if .local is in hostname
    if (hostname.find(".local") == string::npos) hostname += ".local";

    // printf(">> %s\n", hostname.c_str());

    // To retrieve host information
    // host_entry = gethostbyname(hostbuffer);
    host_entry = gethostbyname(hostname.c_str());
    if (host_entry == NULL) throw HostnameError(); //cout << "gethostbyname() error" << endl;

    // To convert an Internet network
    // address into ASCII string
    address = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    if (address.empty()) throw HostnameError(); //cout << "IP address error" << endl;
}

string HostInfo::cleanHostname(){
    // might be easier to use string.erase() and string.find()???
    stringstream ss(hostname);
    string name;
    getline(ss,name,'.');
    return name;
}
