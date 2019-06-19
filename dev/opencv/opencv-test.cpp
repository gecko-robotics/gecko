#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <stdint.h>

using namespace cv;
using namespace std;

int main(){
    Mat img(100, 100, CV_8UC1);
    cout << img.size() << endl;
    vector<uint8_t> buf;
    imencode(".jpg", img, buf);

    cout << buf.size() << endl;
    // int i = 0;
    // for (auto const& v: buf) cout << " " << int(v);
    // cout << endl;

    return 0;
}
