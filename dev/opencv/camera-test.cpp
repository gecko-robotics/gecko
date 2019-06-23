#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <gecko/msgpack/msgs.hpp>


class b_t: public base_t {
public:
    b_t(): base_t(31) {}
    b_t(const cv::Mat img): base_t(31), width(img.cols), height(img.rows) {
        cv::imencode(".jpg", img, buffer);
    }
    b_t(const b_t& a): base_t(31), width(a.width), height(a.height), buffer(a.buffer) {}
    bool operator==(const b_t& v) const {
        if((width == v.width) && (height == v.height) && (buffer == v.buffer))
            return true;
        return false;
    }
    cv::Mat get(){return cv::imdecode(buffer, cv::IMREAD_GRAYSCALE);}
    uint32_t width, height;
    std::vector<uint8_t> buffer;
    MSGPACK_DEFINE(width, height, buffer);
    GECKO_MSG(b_t, 31);
};

using namespace std;
using namespace cv;

int main(){
    Mat img(100, 100, CV_8UC1);
    b_t b(img);
    zmq::message_t m = b.pack();
    b_t c(m);
    cout << (b == c) << endl;
    Mat im = c.get();
    cout << (!cv::norm(im,img,NORM_L1)) << endl;
}
