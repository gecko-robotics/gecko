#include <iostream>
#include <boost/asio.hpp>

using namespace std;

int main(){
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 0);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service, ep);
    cout << socket.local_endpoint().port() << endl;

    return 0;
}
