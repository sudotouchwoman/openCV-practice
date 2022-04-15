#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<string>
#include<iostream>

int main()
{

    cv::Mat imgOriginal;            // input image
    cv::Mat imgGrayscale;           // grayscale of input image
    std::string currFolder = "C:/sakeofShit/VS/myOpenCVPractice/Resources/";
    //std::cout << "Current Folder is :" << currFolder << "\n";
    std::cout << "Please enter an image filename : ";
    std::string img_addr;
    std::cin >> img_addr;

    std::cout << "Searching for " + img_addr << std::endl;

    imgOriginal = cv::imread(currFolder + img_addr);                                             // open image

    if (imgOriginal.empty()) {                                                      // if unable to open image
        std::cerr << "error: image not read from file\n\n";                         // show error message on command line
        //system("pause");
        return(1);                                                                  // and exit program
    }
    else
    {
        std::cout << "Image opened succesfully\nNow converting to binary\n\n";
    }

    cv::cvtColor(imgOriginal, imgGrayscale, cv::COLOR_BGR2GRAY);                    // convert to grayscale

    const int channels = imgGrayscale.channels();
    printf("Number of channels = %d\n", channels);
    printf("Press ESC to exit program\n");

    cv::Mat output;
    imgGrayscale.copyTo(output);                                                    // Just to make sure the Mat objects are of the same size. 
    //Create windows to show image
    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Binary", cv::WINDOW_AUTOSIZE);
    int bin_threshold = 127;
    cv::createTrackbar("Binarisation threshold", "Binary", &bin_threshold, 255);
    while (cv::waitKey(1) != 27)                                                   // hold windows open until user presses ESC
    {
        imgGrayscale.copyTo(output);
        if (channels == 1)
        {
            for (int x = 0; x < imgGrayscale.rows; x++) {
                for (int y = 0; y < imgGrayscale.cols; y++) {
                            // Accesssing values of each pixel
                    if (imgGrayscale.at<uchar>(x, y) >= bin_threshold) {
                            // Setting the pixel values to 255 if it's above the threshold
                        output.at<uchar>(x, y) = 255;
                    }
                    else if (imgGrayscale.at<uchar>(x, y) < bin_threshold) {
                            // Setting the pixel values to 255 if it's below the threshold
                        output.at<uchar>(x, y) = 0;
                    }
                    else {
                            // Just in case
                        printf("The value at (%d, %d) are not right. Value: %d\n", x, y, imgGrayscale.at<uchar>(x, y));
                        break;
                    }
                }
            }
        }
        else if (channels == 3)
        {
            // this is only for  gray scale images
            printf("\tthe image has 3 channels. the function does not support images with 3 channels.\n");
        }
        //  uncomment below to see the original image too
        cv::imshow("Original", imgOriginal);
        cv::imshow("Binary", output);
    }

    return 0;
}