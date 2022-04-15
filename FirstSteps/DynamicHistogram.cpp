#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class Histo
{
		cv::Mat source;
		cv::Mat grayscale;
		cv::Mat Histogram;
		std::vector <int> HistoCeil;
		int hist_w;
		int hist_h;
		int bin_w;
		int HValues;
		bool imgLoad();
		void HValuesInit();
		void GetHV();
		void SetAttributes();
	public:
		Histo();
		Histo(cv::Mat);
		~Histo();
		void DisplayAll();
		cv::Mat & GetSrc();
		cv::Mat & GetHist();
};

cv::Mat& Histo::GetSrc()
{
	return source;
}

cv::Mat& Histo::GetHist()
{
	return Histogram;
}

Histo::Histo()
{
	if (!imgLoad())
	{
		std::cerr << "\tImage load aborted\n";
		return;
	}
	HValuesInit();
	GetHV();
	SetAttributes();
	return;
}

Histo::Histo(cv::Mat img)
{
	source = img;
	HValuesInit();
	GetHV();
	SetAttributes();
	return;
}

Histo::~Histo()
{
	// All vectors and Mat objects are destroyed automatically
	// as they leave their scope
	//delete &source;
	//delete &grayscale;
	//delete &Histogram;
	//delete &HistoCeil;
	return;
}

void Histo::GetHV()
{
	cv::cvtColor(source, grayscale, cv::COLOR_BGR2GRAY);
	// calculate the no of pixels for each intensity values
	for (int y = 0; y < grayscale.rows; y++)
		for (int x = 0; x < grayscale.cols; x++)
			HistoCeil[(int)grayscale.at<uchar>(y, x)]++;
	return;
}

void Histo::HValuesInit()
{
	hist_w = 512;
	hist_h = 400;
	bin_w = cvRound((double)hist_w / 256);
	HValues = 256;
	HistoCeil.resize(HValues);
	// initialize all intensity values to 0
	for (int i = 0; i < HValues; ++i)
	{
		HistoCeil[i] = 0;
	}
	return;
}

void Histo::SetAttributes()
{
	Histogram = cv::Mat(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));
	int max = HistoCeil[0];
	for (int i = 1; i < 256; i++) {
		if (max < HistoCeil[i]) 
			max = HistoCeil[i];
	}
	// normalize the histogram between 0 and histImage.rows
	for (int i = 0; i < HValues; ++i) {
		HistoCeil[i] = int(((double)HistoCeil[i] / max) * Histogram.rows);
	}


	// draw the intensity line for histogram
	for (int i = 0; i < HValues; i++)
	{
		cv::line(Histogram, cv::Point(bin_w * (i), hist_h),
			cv::Point(bin_w * (i), hist_h - HistoCeil[i]),
			cv::Scalar(0, 0, 0), 2, 8, 0);
	}
	return;
}

void Histo::DisplayAll()
{
	if (source.empty())
	{
		std::cerr << "\tNothing to display.\n\n";
		cv::waitKey();
		return;
	}
	const std::string HistWinName = "Intensity Histogram ";
	const std::string ImgWinName = "Image ";
	cv::namedWindow(HistWinName, cv::WINDOW_AUTOSIZE);
	cv::imshow(HistWinName, Histogram);

	cv::namedWindow(ImgWinName, cv::WINDOW_AUTOSIZE);
	cv::imshow(ImgWinName, source);
	return;
}

bool Histo::imgLoad()
{
	std::string pathname;
	const std::string dir = "Resources";
	std::cout << "Enter the filename. Current Directory to search is " << dir << " : ";
	std::cin >> pathname;
	source = cv::imread(dir + "/" + pathname);
	if (source.empty())
	{
		std::cerr << "File empty.\n";
		return false;
	}
	return true;
}

int main()
{	
	//cv::VideoCapture cap;
	//cap.open(0);
	//std::cout << "\tGet video from WebCam and draw an Intensity Histogram of it\n";
	int bin_contrast = 100;
	int bin_intensity = 127;
	std::string pathname;

	const std::string dir = "Resources";
	const std::string savedir("Saves");
	std::cout << "Current Directory to search is \"" << dir << "\"\n";
	std::cout << "Directory for saving is \"" << savedir << "\"\n";
	std::cout << "Enter the filename :\t";
	std::cin >> pathname;
	cv::Mat imgCap = cv::imread(dir + "/" + pathname);
	if (imgCap.empty())
	{
		std::cerr << "File empty.\n";
		system("pause");
		return EXIT_FAILURE;
	}
	else
	{
		std::cerr << "File opened succesfully. Now showing Brightness Histogram of image.\n";
		std::cerr << "Trackbars cann be used to change the contrast & brightness of image.\n";
		std::cerr << "You can see how the Histogram of pixel brightness changes accordingly.\n";
		std::cerr << "Press 'S' for saving the processed image and its histogram into \"" + savedir + "\"\n";
		std::cerr << "Press 'D' for setting Brightness & Contrast to default\n";
	}

	cv::Mat blank;
	cv::Mat empty = cv::Mat::zeros(cv::Size(1, 1000), CV_8UC1);
	cv::namedWindow("TrackbarWin", cv::WINDOW_NORMAL);
	cv::createTrackbar("Contrast", "TrackbarWin", &bin_contrast, 200);
	cv::createTrackbar("Brightness", "TrackbarWin", &bin_intensity, 255);
	int key = 0;
	int id = 0;
	while ((key = cv::waitKey(1)) != 27)
	{
		cv::imshow("TrackbarWin",empty);
		//cap.read(imgCap);
		blank = imgCap + cv::Scalar((127.0 - double(bin_intensity)), (127.0 - double(bin_intensity)), (127.0 - double(bin_intensity)));
		blank *= (double(bin_contrast) / 100);
		Histo H(blank);
		H.DisplayAll();
		if (key == 115)
		{
			cv::imwrite(savedir + "/"  + "Img" + std::to_string(++id) + ".jpg", blank);
			cv::imwrite(savedir + "/" + "Hist" + std::to_string(id) + ".jpg", H.GetHist());
			std::cout << "Save done succesfully!\n";
		}
		if (key == 100)
		{
			bin_contrast = 100;
			bin_intensity = 127;
		}
	}
	std::cout << "\tProcessing finished. ";
	return EXIT_SUCCESS;
}