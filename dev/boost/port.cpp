#include <iostream>
#include <boost/asio.hpp>

using namespace std;
namespace ba = boost::asio;

int main(){
    ba::ip::tcp::endpoint ep(ba::ip::tcp::v4(), 0);
    ba::io_service io_service;
    ba::ip::tcp::socket socket(io_service, ep);
    cout << socket.local_endpoint().port() << endl;

    return 0;
}
