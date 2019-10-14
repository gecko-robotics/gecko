#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <gecko/msgpack/msgs.hpp>


class b_t: public base_t {
public:
    b_t(): base_t(31) {}
    b_t(const cv::Mat img): base_t(31), width(img.cols), height(img.rows), channels(img.channels()) {
        cv::imencode(".jpg", img, buffer);
    }
    b_t(const b_t& a): base_t(31), width(a.width), height(a.height), buffer(a.buffer), channels(a.channels) {}
    bool operator==(const b_t& v) const {
        if((width == v.width) && (height == v.height) && (buffer == v.buffer) && (channels == v.channels))
            return true;
        return false;
    }
    cv::Mat get(){
        return cv::imdecode(buffer, channels == 1 ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR);
    }
    uint32_t width, height, channels;
    std::vector<uint8_t> buffer;
    MSGPACK_DEFINE(width, height, channels, buffer);
    GECKO_MSG(b_t, 31);
};

using namespace std;
using namespace cv;

int main(){
    Mat img(100, 100, CV_8UC3);
    b_t b(img);
    zmq::message_t m = b.pack();
    b_t c(m);
    cout << (b == c) << endl;
    Mat im = c.get();
    cout << (!cv::norm(im,img,NORM_L1)) << endl;

    VideoCapture inputVideo(0);
    if (!inputVideo.isOpened()) {
        exit(1);
    }

    Mat frame;
    namedWindow("frame");
    while(1){
        inputVideo >> frame;
        if (frame.empty()) continue;

        // Test: image -> msg -> image
        b_t m(frame);
        zmq::message_t mm = m.pack();
        b_t mmm(mm);
        Mat img = mmm.get();

        imshow("frame", img);
        char c = (char) waitKey(30);
        if (c == 27) break;
    }

    inputVideo.release();
}
