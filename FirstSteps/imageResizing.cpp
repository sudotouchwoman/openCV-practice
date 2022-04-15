#include <opencv2/imgcodecs/imgcodecs.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include <iostream>

//
//
//

using namespace std;
using namespace cv;



void imResizing(cv::Mat img)
{	
	double newSize;
	Mat Cropped;
	cout << "initial image size is: " << img.size() << "\nwrite the new size (ratio) (0;1):\t";
	cin >> newSize;
	resize(img, Cropped, Size(), newSize, newSize);
	imshow("ORIGINAL", img);
	imshow("RESIZED", Cropped);
	waitKey(0);
	return;
}

void imCropping(cv::Mat img)
{
	Mat Cropped;
	int xpos, ypos, size;
	cout << "write cropping position coordinates:\n\tx:\t"; cin >> xpos;
	cout << "\ty:\t"; cin >> ypos;
	cout << "write square size :\t"; cin >> size;

	Rect ROI(xpos, ypos, size, size);
	Cropped = img(ROI);
	imshow("CROPPED", Cropped);	
	cout << "crop done succesfully\n";
	rectangle(img, ROI, Scalar(255, 0, 255));
	imshow("ANOTHER", img);
	waitKey(0);
	return;
}

const std::string getInput()
{
	std::string path;
	std::cout << "write the file path:\t";
	std::cin >> path;
	return path;
}


int main()
{
	using namespace std;
	using namespace cv;
	string path = "Resources/";
	path += getInput();

	Mat img;

	img = imread(path);

	if (img.empty())
	{
		cout << "image unable to open!\n";
		system("pause");
		exit(EXIT_FAILURE);
	}

	cout << "choose option: cropping or resizing\n";
	cin >> path;
	if (path == "resize")
	{
		imResizing(img);

	}
	else if (path == "crop")
	{
		imCropping(img);
	}
	else
	{
		cout << "no matching commands\n";	
		system("pause");
	}



	return EXIT_SUCCESS;
}