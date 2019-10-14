#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


// void cthread(int t){
//     this_thread::sleep_for(chrono::milliseconds(t));
//     cout << "Hello thread World!" << endl;
// }

class Test {
public:
    void run(){
        cout << "start" << endl;
        thread t1(&Test::cthread2, this);
        thread t2(&Test::cthread, this, 3000);
        t1.join();
        t2.join();
    }
    void cthread2(){
        int t = 500;
        this_thread::sleep_for(chrono::milliseconds(t));
        cout << "Hello thread World!" << endl;
    }
    void cthread(int t){
        this_thread::sleep_for(chrono::milliseconds(t));
        cout << "Hello thread World!" << endl;
    }
};

int main(){
    Test t;
    t.run();

    return 0;
}
