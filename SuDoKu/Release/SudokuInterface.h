#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/ml.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#define ROWS 9
#define W 306
#define CURDIR "C:/sakeofShit/VS/myOpenCVPractice/SuDoKu/Templates/"
#define TEMPLATE "Test.jpg"

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
	KNULL = 48,
	POS = W / ROWS,
	CELLSIZE = W / ROWS - 4,
	MIN_CONTOUR_AREA = 50,
	RESIZED_IMAGE_WIDTH = 10,
	RESIZED_IMAGE_HEIGHT = 10
};

void drawLine(cv::Mat&, cv::Point, cv::Point, int);

class Picker
{
public:
	Picker();
	~Picker();
	void parseKeys();
private:
	std::vector<std::vector<int>> grid;
	cv::Mat imCurrent;
	int xPosition;
	int yPosition;
private:
	bool tryMove(int);
	void putDigit(int);
	void drawLine(cv::Point, cv::Point, int);
};

class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	void solve();
	void setGrid(std::vector<std::vector<int>>&);
private:
	unsigned id;
	unsigned size;
	std::vector<std::vector<int>> grid;
private:
	bool possible(unsigned int, unsigned int, int);
	void out();
	void message();
};

class Classifier
{
public:

public:
	Classifier(const std::string);
	~Classifier();
	int GenData();
	int LoadData();
	std::vector<std::vector<int>> getFinalVector();
	cv::Mat getGrid();
private:
	std::string templatePath;
	cv::Mat matTestingNumbers;
	const static std::vector<int> intValidChars;
	std::vector<std::vector<int>> FinalVector;
private:
	class ContourWithData
	{
	public:
		friend class Classifier;
	private:
		std::vector<cv::Point> ptContour;           // contour
		cv::Rect boundingRect;                      // bounding rect for contour
		double fltArea;                              // area of contour
	private:
		bool checkIfContourIsValid();
		static bool sortByBoundingRectXPosition(const ContourWithData&, const ContourWithData&);
	};
};