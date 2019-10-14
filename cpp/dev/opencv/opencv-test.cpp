#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <stdint.h>

#include <optional>
#include <any>

using namespace cv;
using namespace std;

optional<int> convert(const string& s) {
  try {
	int res = stoi(s);
	return res;
  }
  catch(exception&) {
	return {};
  }
}

int main(){
    Mat img(100, 100, CV_8UC1);
    cout << img.size() << endl;
    vector<uint8_t> buf;
    imencode(".jpg", img, buf);

    cout << buf.size() << endl;
    // int i = 0;
    // for (auto const& v: buf) cout << " " << int(v);
    // cout << endl;

    Mat img2 = imdecode(buf, IMREAD_GRAYSCALE);
    cout << img2.size() << endl;

    int v = convert("123").value_or(0);
    cout << v << endl;

    int v1 = convert("abc").value_or(0);
    cout << v1 << endl;

    vector<any> vv { 1, 2.2, false, "hi!" };

    auto& t = vv[1].type();  // What is being held in this any?
    if (t == typeid(double))
        cout << "We have a double" << "\n";
    else
        cout << "We have a problem!" << "\n";

    cout << any_cast<double>(vv[1]) << endl;
    try {
        cout << any_cast<int>(vv[1]) << endl;
    } catch(bad_any_cast&) {
        cout << "wrong type" << endl;
    }

    return 0;
}
