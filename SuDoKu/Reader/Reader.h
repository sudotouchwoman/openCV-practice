#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "../CreateGrid/GridPicker.h"

using namespace cv;
using namespace std;


class ShapeAnalyser
{
public:
    ShapeAnalyser();
    ShapeAnalyser(int);
    ~ShapeAnalyser();
    void ShapesFromWebcam(int);
    void ShapesFromImage();
private:
    string path;
    Mat imgSource;
    Mat imgExtra;
    void drawLine(Point, Point, int);
    const string getPath();
    void imPProc();
    void showAll();
    void detectShapes();
    vector<vector<Point>> getContours();
};

void ShapeAnalyser::ShapesFromImage()
{
    for (int i = 1; i < ROWS; ++i)
    {
        if (i % 3)
        {
            drawLine(cv::Point((W / ROWS) * i, 0), cv::Point((W / ROWS) * i, W), 1);
            drawLine(cv::Point(0, (W / ROWS) * i), cv::Point(W, (W / ROWS) * i), 1);
        }
        else
        {
            drawLine(cv::Point((W / ROWS) * i, 0), cv::Point((W / ROWS) * i, W), 2);
            drawLine(cv::Point(0, (W / ROWS) * i), cv::Point(W, (W / ROWS) * i), 2);
        }
    }
    imPProc();
    detectShapes();
    showAll();
    if (waitKey(0) != 27)
        imwrite("SuDoKu/Templates/DETECTED.jpg", imgSource);
    return;
}

void ShapeAnalyser::ShapesFromWebcam(int camera = 0)
{
    VideoCapture cap;
    cap.open(camera);
    while (waitKey(1) != 27)
    {
        cap.read(imgSource);
        imPProc();
        detectShapes();
        showAll();
    }
    return;
}

void ShapeAnalyser::drawLine(cv::Point start, cv::Point end, int size)
{
    int thickness = size;
    int lineType = cv::LINE_8;
    line(imgSource,
        start,
        end,
        cv::Scalar(255, 255, 255),
        thickness,
        lineType);
    return;
}


ShapeAnalyser::ShapeAnalyser(int cam)
{
    ShapesFromWebcam(cam);
}

ShapeAnalyser::ShapeAnalyser()
{
    path = "SuDoKu/Templates/SudokuTemplate.jpg";
    imgSource = imread(path);

    if (imgSource.empty())
    {
        cerr << "[i]\timage unable to open!\n";
        system("pause");
        exit(EXIT_FAILURE);
    }
}

void ShapeAnalyser::detectShapes()
{
    vector<vector<Point>> contours = getContours();
    vector<vector<Point>> conApprox(contours.size());
    vector<Rect> boundRects(contours.size());

    boundRects.resize(0);

    for (int i = 0; i < contours.size(); ++i)
    {
        if (contourArea(contours[i]) > 50)
        {
            approxPolyDP(contours[i], conApprox[i], 0.0002 * (arcLength(contours[i], true)), true);
            //drawContours(imgSource, conApprox, i, Scalar(255, 0, 255), 1);
            //drawContours(imgSource, conBig, i, Scalar(0, 0, 255), 1);
            boundRects.push_back(boundingRect(conApprox[i]));
        }
    }

    for (int i = 0; i < boundRects.size(); ++i)
    {
        Mat roiImg;
        roiImg = imgSource(boundRects[i]);
        imwrite("SuDoKu/Templates/TMP_"+ to_string(i) + ".jpg", roiImg);
        //drawContours(imgSource, conBig, i, Scalar(0, 0, 255), 1);
        rectangle(imgSource, boundRects[i].tl(), boundRects[i].br(), Scalar(255, 0, 255), 1);
        imshow("CurrentState", imgSource);
        waitKey(400);
    }
    return;
}

vector<vector<Point>> ShapeAnalyser::getContours()
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(imgExtra, contours, hierarcy, RETR_LIST, CHAIN_APPROX_SIMPLE);
    return contours;
}

const string ShapeAnalyser::getPath()
{
    string path = "SuDoKu/Templates/";
    string input;
    cout << "[i]\twrite the filename:\t";
    cin >> input;
    path += input;
    return path;
}

void ShapeAnalyser::imPProc()
{
    cvtColor(imgSource, imgExtra, COLOR_BGR2GRAY);
    GaussianBlur(imgExtra, imgExtra, Size(3, 3), 3, 0);
    Canny(imgExtra, imgExtra, 25, 50);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
    dilate(imgExtra, imgExtra, kernel);
    return;
}

void ShapeAnalyser::showAll()
{
    imshow("ORIGINAL", imgSource);
    imshow("OUTPUT", imgExtra);
    return;
}

ShapeAnalyser::~ShapeAnalyser()
{
}










//#pragma once
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include <iostream>
//#define RESOURSESDIR "C:/sakeofShit/VS/myOpenCVPractice/SuDoKu/Templates"
//
//using namespace std;
//using namespace cv;
//
//int match(string filename, string templatename)
//{
//    Mat ref = cv::imread(RESOURSESDIR + filename + ".jpg");
//    Mat tpl = cv::imread(RESOURSESDIR + templatename + ".jpg");
//    if (ref.empty() || tpl.empty())
//    {
//        cout << "Error reading file(s)!" << endl;
//        return -1;
//    }
//
//    Mat gref, gtpl;
//    cvtColor(ref, gref, COLOR_BGR2GRAY);
//    cvtColor(tpl, gtpl, COLOR_BGR2GRAY);
//
//    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
//
//    GaussianBlur(ref, ref, Size(5, 5), 5, 0);
//    GaussianBlur(tpl, tpl, Size(5, 5), 5, 0);
//
//    const double low_canny = 75.0;
//
//    Canny(gref, gref, low_canny, low_canny * 3);
//    Canny(gtpl, gtpl, low_canny, low_canny * 3);
//
//    dilate(gref, gref, kernel);
//    dilate(gtpl, gtpl, kernel);
//
//    imshow("file", gref);
//    imshow("template", gtpl);
//
//    Mat res_32f(ref.rows - tpl.rows + 1, ref.cols - tpl.cols + 1, CV_32FC1);
//    matchTemplate(gref, gtpl, res_32f, TM_CCORR_NORMED);
//
//    Mat res;
//    res_32f.convertTo(res, CV_8U, 255.0);
//    imshow("result", res);
//
//    int size = ((tpl.cols + tpl.rows) / 4) * 2 + 1; //force size to be odd
//    adaptiveThreshold(res, res, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, size, -64);
//
//    imshow("result_thresh", res);
//
//    while (1)
//    {
//        double minval, maxval;
//        Point minloc, maxloc;
//        minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);
//        if (maxval > 0)
//        {
//            rectangle(ref, maxloc, Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows), Scalar(0, 255, 0), 2);
//            floodFill(res, maxloc, 0); //mark drawn blob
//        }
//        else
//            break;
//    }
//
//    imshow("final", ref);
//    waitKey(0);
//
//    return 0;
//}
