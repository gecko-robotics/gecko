#pragma once


class RSocket {
public:
    RSocket(){}
    ~RSocket(){
            char mc_addr_str[] = "239.255.255.1"; //argv[1];      /* arg 1: multicast ip address */
            unsigned short mc_port = 10000;
        struct ip_mreq mc_req;        /* multicast request structure */

        mc_req.imr_multiaddr.s_addr = inet_addr(mc_addr_str);
        mc_req.imr_interface.s_addr = htonl(INADDR_ANY);
      /* send a DROP MEMBERSHIP message via setsockopt */
      if ((setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
           (void*) &mc_req, sizeof(mc_req))) < 0) {
        perror("setsockopt() failed");
        exit(1);
      }
      printf("bye ...\n");
      close(sock);
    }
    void init(){
      // int sock;                     /* socket descriptor */
      int flag_on = 1;              /* socket option flag */
      struct sockaddr_in mc_addr;   /* socket address structure */
      char recv_str[MAX_LEN+1];     /* buffer to receive string */
      int recv_len;                 /* length of string received */
      struct ip_mreq mc_req;        /* multicast request structure */
      // char* mc_addr_str;            /* multicast IP address */
      // unsigned short mc_port;       /* multicast port */
      struct sockaddr_in from_addr; /* packet source */
      unsigned int from_len;        /* source addr length */


        char mc_addr_str[] = "239.255.255.1"; //argv[1];      /* arg 1: multicast ip address */
        unsigned short mc_port = 10000; //atoi(argv[2]);    /* arg 2: multicast port number */


      /* create socket to join multicast group on */
      if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket() failed");
        exit(1);
      }

      /* set reuse port to on to allow multiple binds per host */
      if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag_on,
           sizeof(flag_on))) < 0) {
        perror("setsockopt() failed");
        exit(1);
      }

      /* construct a multicast address structure */
      memset(&mc_addr, 0, sizeof(mc_addr));
      mc_addr.sin_family      = AF_INET;
      mc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      mc_addr.sin_port        = htons(mc_port);

      /* bind to multicast address to socket */
      if ((bind(sock, (struct sockaddr *) &mc_addr,
           sizeof(mc_addr))) < 0) {
        perror("bind() failed");
        exit(1);
      }

      /* construct an IGMP join request structure */
      mc_req.imr_multiaddr.s_addr = inet_addr(mc_addr_str);
      mc_req.imr_interface.s_addr = htonl(INADDR_ANY);

      /* send an ADD MEMBERSHIP message via setsockopt */
      if ((setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
           (void*) &mc_req, sizeof(mc_req))) < 0) {
        perror("setsockopt() IP_ADD_MEMBERSHIP failed");
        exit(1);
      }

      /////////////////////////
      u_char loopch = 0;
      if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loopch, sizeof(loopch)) < 0) {
          perror("setsockopt() IP_MULTICAST_LOOP failed");
          exit(1);
      }
      else printf("IP_MULTICAST_LOOP: %c\n", loopch);

      u_char ttl = 1;
      if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
          perror("setsockopt() IP_MULTICAST_TTL failed");
          exit(1);
      }
      /////////////////////////
    }
    void recv(){
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
        printf("Received %d bytes from %s: ", recv_len,
               inet_ntoa(from_addr.sin_addr));
        printf("%s", recv_str);

        // if ((sendto(sock, recv_str, recv_len, 0,
        //      (struct sockaddr *) &mc_addr,
        //      sizeof(mc_addr))) != recv_len) {
        //   perror("sendto() sent incorrect number of bytes");
        //   exit(1);
        // }
        // else printf("Sent: %s\n", recv_str);
    }
    int sock;
};
