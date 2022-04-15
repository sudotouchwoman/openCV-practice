#include "Hist.h"

cv::Mat Histo::getData(const std::string dir, const std::string savedir)
{
	std::string pathname;
	std::cout << "\nCurrent Directory to search is \"" << dir << "\"\n";
	std::cout << "Directory for saving is \"" << savedir << "\"\n";
	std::cout << "Enter the filename :\t";
	std::cin >> pathname;
	cv::Mat image = cv::imread(dir + "/" + pathname);
	if (image.empty())
	{
		std::cerr << "\nFile empty.\n";
		exit(EXIT_FAILURE);
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
	return image;
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
	original = img;
	source = original.clone();
	HValuesInit();
	GetHV();
	SetAttributes();
	return;
}

Histo::~Histo()
{
	// All vectors and Mat objects are destroyed automatically
	// as they leave their scope
	return;
}

cv::Mat& Histo::GetSrc()
{
	return source;
}

cv::Mat& Histo::GetHist()
{
	return Histogram;
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
	bin_w = cvRound((double)hist_w / HValues);
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
	for (int i = 1; i < HValues; i++) {
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
	const std::string text("C: " + std::to_string(this_contrast) + " B: " + std::to_string(254 - this_brightness));

	cv::namedWindow(HistWinName, cv::WINDOW_AUTOSIZE);
	cv::imshow(HistWinName, Histogram);
	cv::namedWindow(ImgWinName, cv::WINDOW_AUTOSIZE);
	cv::hconcat(original, source, source);
	cv::putText(source, text, cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 255));
	cv::imshow(ImgWinName, source);
	return;
}

bool Histo::imgLoad()
{
	std::string pathname;
	const std::string dir = RESOURSESDIR;
	std::cout << "Enter the filename. Current Directory to search is " << dir << " : ";
	std::cin >> pathname;

	original = cv::imread(dir + "/" + pathname);
	source = original.clone();
	
	if (source.empty())
	{
		std::cerr << "File empty.\n";
		return false;
	}
	return true;
}

void Histo::ChangeBrightness(int brightness, void* ptr)
{
	Histo* Instance = (Histo*)ptr;
	Instance->this_brightness = brightness;
	Instance->source = Instance->original + cv::Scalar((127.0 - double(brightness)), (127.0 - double(brightness)), (127.0 - double(brightness)));
	Instance->source *= (double(Instance->this_contrast) / 100.0);
	Instance->HValuesInit();
	Instance->GetHV();
	Instance->SetAttributes();
	Instance->DisplayAll();
	return;
}

void Histo::ChangeContrast(int contrast, void* ptr)
{
	Histo* Instance = (Histo*)ptr;
	Instance->this_contrast = contrast;
	int brightness = Instance->this_brightness;
	Instance->source = Instance->original + cv::Scalar((127.0 - double(brightness)), (127.0 - double(brightness)), (127.0 - double(brightness)));
	Instance->source *= (double(contrast) / 100.0);
	Instance->HValuesInit();
	Instance->GetHV();
	Instance->SetAttributes();
	Instance->DisplayAll();
	return;
}

void Histo::ChangeBrightness(int brightness)
{
	this_brightness = brightness;
	source = original + cv::Scalar((127.0 - double(brightness)), (127.0 - double(brightness)), (127.0 - double(brightness)));
	source *= (double(this_contrast) / 100.0);

	HValuesInit();
	GetHV();
	SetAttributes();
	DisplayAll();

	return;
}

void Histo::ChangeContrast(int contrast)
{
	this_contrast = contrast;
	int brightness = this_brightness;
	source = original + cv::Scalar((127.0 - double(brightness)), (127.0 - double(brightness)), (127.0 - double(brightness)));
	source *= (double(contrast) / 100.0);

	HValuesInit();
	GetHV();
	SetAttributes();
	DisplayAll();

	return;
}