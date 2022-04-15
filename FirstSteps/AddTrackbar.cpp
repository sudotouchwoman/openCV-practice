#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;
cv::Mat src1;
cv::Mat src2;
cv::Mat dst;

static void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);
	cv::addWeighted(src1, alpha, src2, beta, 0.0, dst);
	cv::imshow("Linear Blend", dst);
}

int main(void)
{
	src1 = cv::imread(cv::samples::findFile("LinuxLogo.jpg"));
	src2 = cv::imread(cv::samples::findFile("WindowsLogo.jpg"));
	if (src1.empty()) { std::cout << "Error loading src1 \n"; return -1; }
	if (src2.empty()) { std::cout << "Error loading src2 \n"; return -1; }
	alpha_slider = 0;
	cv::namedWindow("Linear Blend", cv::WINDOW_AUTOSIZE); // Create Window
	cv::createTrackbar("Alpha", "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);
	on_trackbar(alpha_slider, 0);
	cv::waitKey();
	/*for (alpha_slider = 0; alpha_slider <= alpha_slider_max; ++alpha_slider)
	{
		on_trackbar(alpha_slider, 0);
		cv::waitKey(50);
	}*/
	return 0;
}