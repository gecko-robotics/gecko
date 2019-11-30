#include<iostream>
#include <iomanip>
#include <gecko/version.hpp>

using namespace std;

int main(int argc, char* argv[]){
    std::cout << std::setw(6) << argv[0] << " cpp: " <<  __cplusplus << std::endl;
    cout << "gecko version: " << GECKO_VERSION << endl;

    return 0;
}
