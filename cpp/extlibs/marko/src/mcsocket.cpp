#include <marko/mcsocket.hpp>


MCSocket::MCSocket(const std::string& group, int port): save_grp(group), port(port) {
        mc_addr = make(port, INADDR_ANY);

        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        int err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
        if (err < 0){
            throw MulticastError("SSock::setsockopt IP_ADD_MEMBERSHIP failed");
        }
}

MCSocket::~MCSocket(){
    // struct sockaddr_in addr = make(port, "224.3.29.110");
    // sockopt(IPPROTO_IP, IP_DROP_MEMBERSHIP, addr); // FIXME
}

void MCSocket::info(){
    u_char val;
    socklen_t size = sizeof(val);

    printf("Multicast Socket =====================================\n");
    printf("  fd: %d\n", sock);

    getsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &val, &size);
    printf("  loopback: %s\n", val ? "enabled" : "disabled");

    getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, &size);
    printf("  reuse addr: %s\n", val ? "true" : "false");

    getsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &val, &size);
    printf("  reuse port: %s\n", val ? "true" : "false");
}
