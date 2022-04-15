#include <opencv2/highgui.hpp>

int main()
{
	cv::Mat image(200, 200, CV_8UC1, cv::Scalar(0));
	const std::string winname("Hello");
	cv::imshow(winname, image);
	cv::waitKey();
	return EXIT_SUCCESS;
}



