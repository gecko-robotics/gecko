#include <iostream>
// #include <thread>
// #include <chrono>
#include <stdio.h>
#include <unistd.h>  // usleep
// #include <cstdarg>

#include "time.hpp"
#include "node.hpp"
// #include "signals.hpp"
#include "transport.hpp"

// msgpack stuff
#include <msgpack.hpp>
#include "msgs.hpp"
#include "serialization.hpp"

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"

#include <opencv2/opencv.hpp>

using namespace std;
// using namespace cv;

void pub(bool *e){
    cv::Mat src;
    cv::VideoCapture cam;
    cam.open(0);
    cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cv::namedWindow("test", cv::WINDOW_AUTOSIZE);

    Transport<Image> buffer;
    try {
        pid_t pid = getpid();
        printf("-> pub pid: %d\n",pid);
        Publisher *p = Node::advertise("local", "a");
        if (p == nullptr){
            printf("Pubisher failed\n");
            return;
        }

        Rate r(30);

        while(*e){
            // vec_t a(1,2,3);
            // imu_t b(a,a,a);  // new timestamp
            Image b;
            b.width = src.size().width;
            b.height = src.size().height;
            zmq::message_t msg = buffer.pack(b);

            p->pub(msg);
            printf(">> [PUB] sent msg\n");

            // cam >> src;
            bool ok = cam.read(src);
            // if (src.empty()) cout << "Error: no image captured" << endl;
            if (!ok) cout << "Error: no image captured" << endl;
            else cv::imshow( "test", src );
            cv::waitKey(1);

            r.sleep();
        }
        delete p;
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }
    cam.release();

    printf(">> pub bye ...\n");
}




int main(void){
    Node::init();
    pub(&Node::ok);

    return 0;
}
