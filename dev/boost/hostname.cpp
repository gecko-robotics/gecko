#include <iostream>
#include <boost/asio.hpp>
#include <string>

using namespace std;
namespace baip = boost::asio::ip;

int main(){
  string h = baip::host_name();
  cout << "hostname: " << h << endl;
}
