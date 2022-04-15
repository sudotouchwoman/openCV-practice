#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

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
    const string getPath();
    void imPProc();
    void showAll();
    void detectShapes();
    vector<vector<Point>> getContours();
};

void ShapeAnalyser::ShapesFromImage()
{
    imPProc();
    detectShapes();
    showAll();
    waitKey(0);
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

ShapeAnalyser::ShapeAnalyser(int cam)
{
    ShapesFromWebcam(cam);
}

ShapeAnalyser::ShapeAnalyser()
{
    path = getPath();
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
    //conBig(contours.size()),            conBig.push_back(contours[i]);
    vector<vector<Point>> contours = getContours();
    vector<vector<Point>> conApprox(contours.size());
    vector<Rect> boundRects(contours.size());

    boundRects.resize(0);

    for (int i = 0; i < contours.size(); ++i)
    {
        if (contourArea(contours[i]) > 2000)
        {
            approxPolyDP(contours[i], conApprox[i], 0.0002 * (arcLength(contours[i], true)), true);
            //drawContours(imgSource, conApprox, i, Scalar(255, 0, 255), 1);
            //drawContours(imgSource, conBig, i, Scalar(0, 0, 255), 1);
            boundRects.push_back(boundingRect(conApprox[i]));
        }
    }

    for (int i = 0; i < boundRects.size(); ++i)
    {
        //drawContours(imgSource, conBig, i, Scalar(0, 0, 255), 1);
        rectangle(imgSource, boundRects[i].tl(), boundRects[i].br(), Scalar(255, 255, 0), 1);
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
    //waitKey(0);
    return;
}

ShapeAnalyser::~ShapeAnalyser()
{
}

/////////////////  SHAPES RECOGNITION  //////////////////////

int main() 
{
    ShapeAnalyser test;
    test.ShapesFromImage();
    return 0;
}