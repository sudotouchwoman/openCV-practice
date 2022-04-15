#include "SudokuInterface.h"

// possible chars we are interested in are digits 0 through 9 and capital letters A through Z, put these in vector intValidChars
const std::vector<int> Classifier::intValidChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z' };

void drawLine(cv::Mat& imCurrent, cv::Point start, cv::Point end, int size, int color)
{
    int thickness = size;
    int lineType = cv::LINE_8;
    line(imCurrent,
        start,
        end,
        cv::Scalar(color),
        thickness,
        lineType);
    return;
}

Classifier::Classifier(const std::string Path)
{
    const std::string dirpath(CURDIR);
    templatePath = dirpath + Path;
}

Classifier::~Classifier()
{
}

int Classifier::GenData()
{
    cv::Mat imgTrainingNumbers;         // input image
    cv::Mat imgGrayscale;               // 
    cv::Mat imgBlurred;                 // declare various images
    cv::Mat imgThresh;                  //
    cv::Mat imgThreshCopy;              //

    std::vector<std::vector<cv::Point> > ptContours;        // declare contours vector
    std::vector<cv::Vec4i> v4iHierarchy;                    // declare contours hierarchy

    cv::Mat matClassificationInts;      // these are our training classifications, note we will have to perform some conversions before writing to file later
                                        // these are our training images, due to the data types that the KNN object KNearest requires, we have to declare a single Mat,
                                        // then append to it as though it's a vector, also we will have to perform some conversions before writing to file later
    cv::Mat matTrainingImagesAsFlattenedFloats;
    imgTrainingNumbers = cv::imread(templatePath);          // read in training numbers image

    if (imgTrainingNumbers.empty()) {                               // if unable to open image
        std::cout << "error: image not read from file\n\n";         // show error message on command line
        return(EXIT_FAILURE);                                                  // and exit program
    }

    cv::cvtColor(imgTrainingNumbers, imgGrayscale, cv::COLOR_BGR2GRAY);        // convert to grayscale

    for (int i = 1; i < ROWS; ++i)
    {
        if (i % 3)
        {
            drawLine(imgGrayscale,cv::Point(Usage::POS * i, 0), cv::Point(Usage::POS * i, W), 1, Usage::BGRND);
            drawLine(imgGrayscale,cv::Point(0, Usage::POS * i), cv::Point(W, Usage::POS * i), 1, Usage::BGRND);
        }
        else
        {
            drawLine(imgGrayscale,cv::Point(Usage::POS * i, 0), cv::Point(Usage::POS * i, W), 2, Usage::BGRND);
            drawLine(imgGrayscale,cv::Point(0, Usage::POS * i), cv::Point(W, Usage::POS * i), 2, Usage::BGRND);
        }
    }

    cv::GaussianBlur(imgGrayscale,              // input image
        imgBlurred,                             // output image
        cv::Size(5, 5),                         // smoothing window width and height in pixels
        0);                                     // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value

                                                // filter image from grayscale to black and white
    cv::adaptiveThreshold(imgBlurred,           // input image
        imgThresh,                              // output image
        255,                                    // make pixels that pass the threshold full white
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,         // use gaussian rather than mean, seems to give better results
        cv::THRESH_BINARY_INV,                  // invert so foreground will be white, background will be black
        11,                                     // size of a pixel neighborhood used to calculate threshold value
        2);                                     // constant subtracted from the mean or weighted mean

    cv::imshow("imgThresh", imgThresh);         // show threshold image for reference

    imgThreshCopy = imgThresh.clone();          // make a copy of the thresh image, this in necessary b/c findContours modifies the image

    cv::findContours(imgThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
        ptContours,                             // output contours
        v4iHierarchy,                           // output hierarchy
        cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
        cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

    for (int i = 0; i < ptContours.size(); i++)
    {                                                                       // for each contour
        if (cv::contourArea(ptContours[i]) > Usage::MIN_CONTOUR_AREA)
        {                                                                   // if contour is big enough to consider
            cv::Rect boundingRect = cv::boundingRect(ptContours[i]);        // get the bounding rect

            cv::rectangle(imgTrainingNumbers, boundingRect, cv::Scalar(0, 0, 255), 2);      
                                                                        // draw red rectangle around each contour as we ask user for input
            cv::Mat matROI = imgThresh(boundingRect);                   // get ROI image of bounding rect
            cv::Mat matROIResized;
            cv::resize(matROI, matROIResized, cv::Size(Usage::RESIZED_IMAGE_WIDTH, Usage::RESIZED_IMAGE_HEIGHT));     
                                                                        // resize image, this will be more consistent for recognition and storage
            cv::imshow("matROI", matROI);                               // show ROI image for reference
            cv::imshow("matROIResized", matROIResized);                 // show resized ROI image for reference
            cv::imshow("imgTrainingNumbers", imgTrainingNumbers);       // show training numbers image, this will now have red rectangles drawn on it

            int intChar = cv::waitKey(0);           // get key press
            if (intChar == 27)
            {                                       // if esc key was pressed
                return(EXIT_FAILURE);               // exit program
            }
            else if (std::find(intValidChars.begin(), intValidChars.end(), intChar) != intValidChars.end())
            {                                                           // else if the char is in the list of chars we are looking for . . .
                matClassificationInts.push_back(intChar);               // append classification char to integer list of chars

                cv::Mat matImageFloat;                                  // now add the training image (some conversion is necessary first) . . .
                matROIResized.convertTo(matImageFloat, CV_32FC1);       // convert Mat to float

                cv::Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);               // flatten

                matTrainingImagesAsFlattenedFloats.push_back(matImageFlattenedFloat);       // add to Mat as though it was a vector, this is necessary due to the
                                                                                            // data types that KNearest.train accepts
            }   // end if
        }   // end if
    }   // end for

    std::cout << "training complete\n\n";

    // save classifications to file ///////////////////////////////////////////////////////

    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::WRITE);                   // open the classifications file
    if (fsClassifications.isOpened() == false) {                                                        // if the file was not opened successfully
        std::cerr << "error, unable to open training classifications file, exiting program\n\n";        // show error message
        return(EXIT_FAILURE);                                                                           // and exit program
    }

    fsClassifications << "classifications" << matClassificationInts;        // write classifications into classifications section of classifications file
    fsClassifications.release();                                            // close the classifications file
    // save training images to file ///////////////////////////////////////////////////////

    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::WRITE);                     // open the training images file
    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        std::cerr << "error, unable to open training images file, exiting program\n\n";         // show error message
        return(EXIT_FAILURE);                                                                   // and exit program
    }

    fsTrainingImages << "images" << matTrainingImagesAsFlattenedFloats;         // write training images into images section of images file
    fsTrainingImages.release();                                                 // close the training images file

	return(EXIT_SUCCESS);
}

int Classifier::LoadData()
{
    std::vector<ContourWithData> allContoursWithData;           // declare empty vectors,
    std::vector<ContourWithData> validContoursWithData;         // we will fill these shortly
    // read in training classifications ///////////////////////////////////////////////////
    cv::Mat matClassificationInts;
                            // we will read the classification numbers into this variable as though it is a vector
    cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);                
                                                                                                    // open the classifications file
    if (fsClassifications.isOpened() == false) {                                                    // if the file was not opened successfully
        std::cout << "error, unable to open training classifications file, exiting program\n\n";    // show error message
        return(EXIT_FAILURE);                                                                       // and exit program
    }
    fsClassifications["classifications"] >> matClassificationInts;      // read classifications section into Mat classifications variable
    fsClassifications.release();                                        // close the classifications file

    // read in training images ////////////////////////////////////////////////////////////

    cv::Mat matTrainingImagesAsFlattenedFloats;         // we will read multiple images into this single image variable as though it is a vector

    cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);          
                                                                                                // open the training images file
    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
        return(EXIT_FAILURE);                                                                   // and exit program
    }
    fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
    fsTrainingImages.release();                                                 // close the traning images file

    // train //////////////////////////////////////////////////////////////////////////////

    cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());    
                    // instantiate the KNN object
                    // finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
                    // even though in reality they are multiple images / numbers
    kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

    // test ///////////////////////////////////////////////////////////////////////////////

    matTestingNumbers = cv::imread(templatePath);
                                                                    // read in the test numbers image
    if (matTestingNumbers.empty()) {                                // if unable to open image
        std::cout << "error: image not read from file\n\n";         // show error message on command line
        return(EXIT_FAILURE);                                                  // and exit program
    }

    cv::Mat matGrayscale;           //
    cv::Mat matBlurred;             // declare more image variables
    cv::Mat matThresh;              //
    cv::Mat matThreshCopy;          //

    cv::cvtColor(matTestingNumbers, matGrayscale, cv::COLOR_BGR2GRAY);         // convert to grayscale
    
    for (int i = 1; i < ROWS; ++i)
    {
        if (i % 3)
        {
            drawLine(matGrayscale, cv::Point(Usage::POS * i, 0), cv::Point(Usage::POS * i, W), 1, Usage::BGRND);
            drawLine(matGrayscale, cv::Point(0, Usage::POS * i), cv::Point(W, Usage::POS * i), 1, Usage::BGRND);
        }
        else
        {
            drawLine(matGrayscale, cv::Point(Usage::POS * i, 0), cv::Point(Usage::POS * i, W), 2, Usage::BGRND);
            drawLine(matGrayscale, cv::Point(0, Usage::POS * i), cv::Point(W, Usage::POS * i), 2, Usage::BGRND);
        }
    }
                                                                        // blur
    cv::GaussianBlur(matGrayscale,              // input image
        matBlurred,                // output image
        cv::Size(5, 5),            // smoothing window width and height in pixels
        0);                        // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value

                                   // filter image from grayscale to black and white
    cv::adaptiveThreshold(matBlurred,                           // input image
        matThresh,                            // output image
        255,                                  // make pixels that pass the threshold full white
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,       // use gaussian rather than mean, seems to give better results
        cv::THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
        11,                                   // size of a pixel neighborhood used to calculate threshold value
        2);                                   // constant subtracted from the mean or weighted mean

    matThreshCopy = matThresh.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image

    std::vector<std::vector<cv::Point> > ptContours;        // declare a vector for the contours
    std::vector<cv::Vec4i> v4iHierarchy;                    // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

    cv::findContours(matThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
        ptContours,                             // output contours
        v4iHierarchy,                           // output hierarchy
        cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
        cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

    for (int i = 0; i < ptContours.size(); i++) {               // for each contour
        ContourWithData contourWithData;                                                    // instantiate a contour with data object
        contourWithData.ptContour = ptContours[i];                                          // assign contour to contour with data
        contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);         // get the bounding rect
        contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);               // calculate the contour area
        allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
    }

    for (int i = 0; i < allContoursWithData.size(); i++) {                      // for all contours
        if (allContoursWithData[i].checkIfContourIsValid()) {                   // check if valid
            validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
        }
    }
    // sort contours from left to right
    std::sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);

    FinalVector.resize(9);
    for (size_t i = 0; i < FinalVector.size(); ++i)
    {
        std::vector<int> empty;
        empty.resize(9);
        FinalVector[i] = (empty);
    }

    for (int i = 0; i < validContoursWithData.size(); i++)
    {                                                   // for each contour
                                                        // draw a green rect around the current char
        cv::rectangle(matTestingNumbers,                // draw rectangle on original image
            validContoursWithData[i].boundingRect,      // rect to draw
            cv::Scalar(0, 255, 0),                      // green
            2);                                         // thickness

        cv::Mat matROI = matThresh(validContoursWithData[i].boundingRect);
        // get ROI image of bounding rect
        // resize image, this will be more consistent for recognition and storage
        cv::Mat matROIResized;
        cv::resize(matROI, matROIResized, cv::Size(Usage::RESIZED_IMAGE_WIDTH, Usage::RESIZED_IMAGE_HEIGHT));

        // convert Mat to float, necessary for call to find_nearest
        cv::Mat matROIFloat;
        matROIResized.convertTo(matROIFloat, CV_32FC1);

        cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);
        cv::Mat matCurrentChar(0, 0, CV_32F);

        // finally we can call find_nearest !!!
        kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     

        float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);
        // for debugging purposes print out the position of each digit
        std::cerr << "Y: " << validContoursWithData[i].boundingRect.y << " X: " << validContoursWithData[i].boundingRect.x << '\n';
        cv::waitKey(40);
        // put the classified int into the corresponding cell of vector
        FinalVector[validContoursWithData[i].boundingRect.y / Usage::POS][validContoursWithData[i].boundingRect.x / Usage::POS] = (int(fltCurrentChar)) - 48;
    }

    for (int i = 1; i < W; ++i)
    {
        if (i % 3)
        {
            drawLine(matTestingNumbers, cv::Point(Usage::POS * i, 0), cv::Point(Usage::POS * i, W), 1, Usage::FGRND);
            drawLine(matTestingNumbers, cv::Point(0, Usage::POS * i), cv::Point(W, Usage::POS * i), 1, Usage::FGRND);
        }
        else
        {
            drawLine(matTestingNumbers, cv::Point(Usage::POS* i, 0), cv::Point(Usage::POS * i, W), 2, Usage::FGRND);
            drawLine(matTestingNumbers, cv::Point(0, Usage::POS * i), cv::Point(W, Usage::POS * i), 2, Usage::FGRND);
        }
    }

	return EXIT_SUCCESS;
}

std::vector<std::vector<int>> Classifier::getFinalVector()
{
    return this->FinalVector;
}

cv::Mat Classifier::getGrid()
{
    // show input image with green boxes drawn around found digits
    cv::imshow("Sudoku Grid", matTestingNumbers);
    cv::waitKey();
    return this->matTestingNumbers;
}

bool Classifier::ContourWithData::checkIfContourIsValid() {             // obviously in a production grade program
	if (fltArea < Usage::MIN_CONTOUR_AREA) return false;    // we would have a much more robust function for 
	return true;                                            // identifying if a contour is valid !!
}

bool Classifier::ContourWithData::sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight)
{															// this function allows us to sort
	return((cwdLeft.boundingRect.y * W) + (cwdLeft.boundingRect.x) < (cwdRight.boundingRect.y * W) + (cwdRight.boundingRect.x));                                                   // the contours from left to right
}