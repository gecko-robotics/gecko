#include <marko/bsocket.hpp>

BCSocket::BCSocket(int port): port(port){
    sockopt(SOL_SOCKET, SO_BROADCAST, 1);
    bc_addr = make(port, INADDR_BROADCAST);
}

void BCSocket::info(){
    printf("Broadcast Socket =====================================\n");
    printf("  fd: %d\n", sock);
}
