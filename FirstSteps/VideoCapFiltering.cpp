#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>

using namespace cv;


int main() {

    VideoCapture cap;
    cap.open(0);

    while (waitKey(20) != 27) {

        Mat src;
        Mat threshold;

        cap.read(src);

        inRange(src, Scalar(0, 0, 50), Scalar(255, 75, 255), threshold);
        imshow("thr", threshold);
        imshow("hsv", src);
    }
    return 0;
}