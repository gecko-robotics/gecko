#include<iostream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]){
    std::cout << std::setw(6) << argv[0] << " cpp: " <<  __cplusplus << std::endl;

    return 0;
}
