#include <opencv2/imgcodecs/imgcodecs.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include <iostream>

//
//
//

cv::Mat findEdges(cv::Mat);

const std::string getInput()
{
	std::string path;
	std::cout << "write the file path:\t";
	std::cin >> path;
	return path;
}

void imProcessing(cv::Mat img)
{
	using namespace cv;
	Mat imgGray;
	Mat imgBlurred;

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(img, imgBlurred, Size(3, 3), 5, 0);

	/*imshow("ORIGINAL", img);
	imshow("GRAY", imgGray);
	imshow("BLURRED", imgBlurred);
	waitKey(0);*/

	imshow("EDGES FOUND", findEdges(imgBlurred));
	if (waitKey(0) == 27)
	{
		std::cout << "test passed succesfully!\n";
	}
	return;
}

cv::Mat findEdges(cv::Mat img)
{
	cv::Mat imgEdges;
	cv::Canny(img, imgEdges, 25, 75);
	return imgEdges;
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
		cerr << "image unable to open!\n";
		exit(EXIT_FAILURE);
	}

	imProcessing(img);
	//system("pause");
	return EXIT_SUCCESS;
}