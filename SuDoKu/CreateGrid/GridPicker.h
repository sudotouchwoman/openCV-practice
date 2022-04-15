#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#define SAVEDIR "C:/sakeofShit/VS/myOpenCVPractice/"
#define W 306
#define ROWS 9

enum Usage
{
	BGRND = 255,
	FGRND = 0,
	KEYUP = 119,
	KEYLEFT = 97,
	KEYRIGHT = 100,
	KEYDOWN = 115,
	K1 = 49,
	K2 = 50,
	K3 = 51,
	K4 = 52,
	K5 = 53,
	K6 = 54,
	K7 = 55,
	K8 = 56,
	K9 = 57,
	POS = W / ROWS,
	CELLSIZE = W/ROWS - 4
};

class Picker
{
	public:
		Picker();
		~Picker();
		void parseKeys();
	private:
		std::vector<std::vector<int>> grid;
		//cv::Mat imCopy;
		cv::Mat imCurrent;
		//std::vector<cv::Mat> DigitTemplates;
		int xPosition;
		int yPosition;
	private:
		bool tryMove(int);
		void putDigit(int);
		void drawLine(cv::Point, cv::Point, int);
};