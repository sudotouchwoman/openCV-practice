#include "Hist.h"
#define RESDIR "C:/sakeofShit/VS/myOpenCVPractice/Resources"
#define SAVEDIR "C:/sakeofShit/VS/myOpenCVPractice/Saves"

int main()
{
	int bin_contrast = 100;
	int bin_intensity = 127;
	std::string pathname;
	const std::string dir(RESDIR);
	const std::string savedir(SAVEDIR);
	std::cout << "\nCurrent Directory to search is \"" << dir << "\"\n";
	std::cout << "Directory for saving is \"" << savedir << "\"\n";
	std::cout << "Enter the filename :\t";
	std::cin >> pathname;
	cv::Mat imgCap = cv::imread(dir + "/" + pathname);
	if (imgCap.empty())
	{
		std::cerr << "\nFile empty.\n";
		return EXIT_FAILURE;
	}
	else
	{
		std::cerr << "\nFile opened succesfully. Now showing Brightness Histogram of image.\n";
		std::cerr << "Trackbars can be used to change the contrast & brightness of image.\n";
		std::cerr << "You can see how the Histogram of pixel brightness changes accordingly.\n";
		std::cerr << "Press 'S' for saving the processed image and its histogram into \"" + savedir + "\"\n";
		std::cerr << "Press 'D' for setting Brightness & Contrast to default\n";
		std::cerr << "Press +/- to increase/decrease contrast by 1 \n";
		std::cerr << "Press 'Shift' & +/- to increase/decrease brightness by 1 \n";
	}

	//cv::Mat empty = cv::Mat::zeros(cv::Size(1, 1000), CV_8UC1);
	//cv::namedWindow("TrackbarWin", cv::WINDOW_NORMAL);
	Histo H(imgCap);
	//cv::createTrackbar("Contrast", "TrackbarWin", &bin_contrast, 200, Histo::ChangeContrast, (void*)&H);
	//cv::createTrackbar("Brightness", "TrackbarWin", &bin_intensity, 255, Histo::ChangeBrightness, (void*)&H);
	H.DisplayAll();
	int key = 0;
	int id = 0;
	//cv::imshow("TrackbarWin", empty);
	while ((key = cv::waitKey()) != 27)
	{
		switch (key)
		{
		case 45:
			H.ChangeBrightness(++bin_intensity);
			//Histo::ChangeBrightness(++bin_intensity, (void*)&H);
			break;
		case 61:
			H.ChangeBrightness(--bin_intensity);
			//Histo::ChangeBrightness(--bin_intensity, (void*)&H);
			break;
		case 95:
			H.ChangeContrast(--bin_contrast);
			//Histo::ChangeContrast(--bin_contrast, (void*)&H);
			break;
		case 43:
			H.ChangeContrast(++bin_contrast);
			//Histo::ChangeContrast(++bin_contrast, (void*)&H);
			break;
		case 115 :
			cv::imwrite(savedir + "/" + "Img" + std::to_string(++id) + ".jpg", H.GetSrc());
			cv::imwrite(savedir + "/" + "Hist" + std::to_string(id) + ".jpg", H.GetHist());
			std::cout << "Save done succesfully!\n";
			break;
		case 100:
			Histo::ChangeBrightness(bin_intensity = 127, (void*)&H);
			Histo::ChangeContrast(bin_contrast = 100, (void*)&H);
			std::cout << "Set to default!\n";
			break;
		default:
			std::cout << "Nothing to do here! KeyCode:\t" << key << "\n";
			break;
		}
	}
	std::cout << "\nProgramm finished. (0)\n";
	return EXIT_SUCCESS;
}