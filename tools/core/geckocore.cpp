#include "gecko/gecko.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>


int getFreePort(void){
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 0);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service, ep);
    return socket.local_endpoint().port();
}

class Core {
public:
    Core(std::string k): key(k)

protected:
    std::string key;
};


using namespace std;

int main(){
    return 0;
}
