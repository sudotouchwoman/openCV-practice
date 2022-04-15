#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/////////////////  Images  //////////////////////

int main() {

    string path = "Resources/";
    string input;

    cout << "[i]\twrite the filename:\t";
    cin >> input;
    path += input;
    path += ".png";

    Mat img = imread(path);

    if (img.empty())
    {
        cerr << "[i]\timage unable to open.\n";
        cin.get();
        return -1;
    }
    else
    {
        imshow(input, img);
        cout << "[i]\timage opened succesfully.\n";
        if (27 == waitKey(0))
        {
            cout << "[i]\ttest completed.\n";
        }
    }
    return 0;

}