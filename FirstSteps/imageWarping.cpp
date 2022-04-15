#include <opencv2/imgcodecs/imgcodecs.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include <iostream>

//	IMAGE WARPING EXAMPLE WITH CARDS PIC
//	GETS IMAGE PESPECTIVE MATRIX AND
//	WARPS IT TO A PLAIN ONE (EXECUTE & SEE)

using namespace std;
using namespace cv;

const std::string getInput()
{
	/*std::string path;
	std::cout << "write the file path:\t";
	std::cin >> path;
	return path;*/
	return "cards.jpg";
}


void Warp(Mat img)
{
	Mat perspectiveMatrix;
	Mat imWarped;
	float width = 300;
	float heigtht = 400;
	Point2f src[4] = { {779,110},{1019,89},{839,355},{1116,327} };
	Point2f dst[4] = { {0.0f,0.0f},{width,0.0f},{0.0f,heigtht},{width,heigtht} };

	perspectiveMatrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imWarped, perspectiveMatrix, Point(width,heigtht));

	while ( waitKey(1) != 27)
	{
		imshow("ORIGINAL", img);
		imshow("WARPED", imWarped);
	}
	
	return;
}


int main()
{
	Mat img;
	string path = "Resources/";

	path += getInput();
	img = imread(path);

	if (img.empty())
	{
		cout << "image unable to open!\n";
		system("pause");
		exit(EXIT_FAILURE);
	}

	Warp(img);

	return EXIT_SUCCESS;
}