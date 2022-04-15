#pragma once
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#define RESOURSESDIR "C:/sakeofShit/VS/myOpenCVPractice/Resources"

class Histo
{
private:
	cv::Mat source;
	cv::Mat original;
	cv::Mat grayscale;
	cv::Mat Histogram;
	std::vector <int> HistoCeil;
	int hist_w;
	int hist_h;
	int bin_w;
	const int HValues = 256;
	int this_brightness = 127;
	int this_contrast = 100;
private:
	bool imgLoad();
	void HValuesInit();
	void GetHV();
	void SetAttributes();
public:
	Histo();
	Histo(cv::Mat);
	~Histo();
	void DisplayAll();
	cv::Mat& GetSrc();
	cv::Mat& GetHist();
	static void ChangeBrightness(int, void*);
	static void ChangeContrast(int, void*);
	static cv::Mat getData(const std::string dir, const std::string savedir);
	void ChangeBrightness(int);
	void ChangeContrast(int);
};
