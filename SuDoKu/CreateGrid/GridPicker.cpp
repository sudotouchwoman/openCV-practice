#pragma once
#include "GridPicker.h"

Picker::Picker()
{
    xPosition = 0;
    yPosition = 0;
    cv::Mat img(W, W, CV_8UC3, cv::Scalar(Usage::BGRND, Usage::BGRND, Usage::BGRND));
    imCurrent = img.clone();
    for (int i = 1; i < ROWS; ++i)
    {
        if (i % 3)
        {
            drawLine(cv::Point((W / ROWS) * i, 0), cv::Point((W / ROWS) * i, W),1);
            drawLine(cv::Point(0, (W / ROWS) * i), cv::Point(W, (W / ROWS) * i),1);
        }
        else
        {
            drawLine(cv::Point((W / ROWS) * i, 0), cv::Point((W / ROWS) * i, W),2);
            drawLine(cv::Point(0, (W / ROWS) * i), cv::Point(W, (W / ROWS) * i),2);
        }
    }
}

Picker::~Picker()
{

}

void Picker::parseKeys()
{
    int Key = 0;
    while ((Key = cv::waitKey()) != 27)
    {
        tryMove(Key);

        if (Key >= 48 && Key <= 57)
        {
            putDigit(Key);
        }
        cv::imshow("GRID", imCurrent);
    }
    cv::imwrite(std::string(SAVEDIR) + std::string("Test.jpg"), imCurrent);
    return;
}

void Picker::drawLine(cv::Point start, cv::Point end, int size)
{
    int thickness = size;
    int lineType = cv::LINE_8;
    line(imCurrent,
        start,
        end,
        cv::Scalar(Usage::FGRND, Usage::FGRND, Usage::FGRND),
        thickness,
        lineType);
    return;
}

bool Picker::tryMove(int Key)
{
    switch (Key)
    {
    case Usage::KEYUP:
        if (yPosition > 0)
        {
            --yPosition;
            return true;
        }
        break;
    case Usage::KEYDOWN:
        if (yPosition < ROWS - 1)
        {
            ++yPosition;
            return true;
        }
        break;
    case Usage::KEYLEFT:
        if (xPosition > 0)
        {
            --xPosition;
            return true;
        }
        break;
    case Usage::KEYRIGHT:
        if (xPosition < ROWS - 1)
        {
            ++xPosition;
            return true;
        }
        break;
    case Usage::K1:
    case Usage::K2:
    case Usage::K3:
    case Usage::K4:
    case Usage::K5:
    case Usage::K6:
    case Usage::K7:
    case Usage::K8:
    case Usage::K9:
        return false;
        break;
    default:
        return false;
        break;
    }
}

void Picker::putDigit(int Digit)
{
    cv::Rect area((xPosition * Usage::POS + 2), ((yPosition) * Usage::POS + 2), Usage::CELLSIZE, Usage::CELLSIZE);
    cv::rectangle(imCurrent, area, cv::Scalar(Usage::BGRND, Usage::BGRND, Usage::BGRND),-1);
    if (Digit == 48)
        return;
    cv::putText(
        imCurrent, std::to_string(9 - (Usage::K9 - Digit)),
        cv::Point(xPosition * Usage::POS + Usage::CELLSIZE / 2 - 3, (yPosition + 1) * Usage::POS - 10),
        cv::FONT_HERSHEY_COMPLEX_SMALL,
        1.0,
        cv::Scalar(Usage::FGRND, Usage::FGRND, Usage::FGRND),
        1
    );
    return;
}
