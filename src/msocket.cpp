#include "msocket.hpp"

#include <sys/types.h>  /* for type definitions */
#include <sys/socket.h> /* for socket API calls */
#include <netinet/in.h> /* for address structs */
// #include <arpa/inet.h>  /* for sockaddr_in */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for strlen() */
#include <unistd.h>     /* for close() */
#include <string>

using namespace std;

void SSocket::init(string mc_addr_str, uint16_t mc_port, uint8_t mc_ttl){
  /* create a socket for sending to the multicast address */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket() SOCK_DGRAM failed");
    exit(1);
  }

  /* set the TTL (time to live/hop count) for the send */
  if ((setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
       (void*) &mc_ttl, sizeof(mc_ttl))) < 0) {
    perror("setsockopt() IP_MULTICAST_TTL failed");
    exit(1);
  }

  /* construct a multicast address structure */
  memset(&mc_addr, 0, sizeof(mc_addr));
  mc_addr.sin_family      = AF_INET;
  mc_addr.sin_addr.s_addr = inet_addr(mc_addr_str.c_str());
  mc_addr.sin_port        = htons(mc_port);
}

bool SSocket::ready(int msec){
    struct timeval tv;
    fd_set readfds;
    // int err;
    long usec = msec * 1000;

    if (usec > 999999){
        printf("====================================\n");
        printf(" Beacon::ready msec < 1,000 msec\n");
        printf("====================================\n\n");
        usec = 999999;
    }

    tv.tv_sec = 0;
    tv.tv_usec = usec;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    // don't care about writefds and exceptfds:
    if (select(sock+1, &readfds, NULL, NULL, &tv) < 0)
        perror("ready select issue");

    if (FD_ISSET(sock, &readfds)) return true;
    return false;
}

std::string SSocket::recv(int msec){
    uint16_t recv_len = 0;
    uint16_t MAX_LEN = 1024;
    unsigned int from_len;
    char recv_str[1024];
    struct sockaddr_in from_addr;
    std::string msg;

    if(ready(msec)) // check to see if data is waiting using select()
    {
        /* clear the receive buffers & structs */
        memset(recv_str, 0, sizeof(recv_str));
        from_len = sizeof(from_addr);
        memset(&from_addr, 0, from_len);

        /* block waiting to receive a packet */
        if ((recv_len = recvfrom(sock, recv_str, MAX_LEN, 0,
             (struct sockaddr*)&from_addr, &from_len)) < 0) {
          perror("recvfrom() failed");
          exit(1);
        }

        /* output received string */
        printf("Received %d bytes from %s\n", recv_len,
               inet_ntoa(from_addr.sin_addr));
        // printf("%s\n", recv_str);
        msg = recv_str;
    }
    else printf("** no joy **\n");

    return msg;
}

bool SSocket::send(std::string msg){
    /* send string to multicast address */
    if ((sendto(sock, msg.c_str(), msg.size(), 0,
         (struct sockaddr *) &mc_addr,
         sizeof(mc_addr))) != msg.size()) {
      perror("sendto() sent incorrect number of bytes");
      // exit(1);
      return false;
    }
    // sleep(1);
    return true;
}
