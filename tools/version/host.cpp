#include <iostream>
#include <iomanip>

#include <gecko/gecko.hpp>

using namespace std;

int main(int argc, char* argv[]){
    HostInfo host;
    cout << "Host Info: " << host.hostname << " [" << host.address <<"]" << endl;

    return 0;
}
