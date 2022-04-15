#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>

int main()
{
    cv::Mat imgOriginal;                                                    // Input image
    cv::Mat imgGrayscale;                                                   // Grayscale of the input image
    const std::string currFolder = "C:/sakeofShit/VS/myOpenCVPractice/Resources/";
    std::cout << "Current Folder is :" << currFolder << "\n";               // Path to folder with image samples
    std::cout << "Please enter an image filename : ";
    std::string img_addr;
    std::cin >> img_addr;
    std::cout << "Searching for " + img_addr << std::endl;

    imgOriginal = cv::imread(currFolder + img_addr);                        // Open image

    if (imgOriginal.empty()) {                                              // If unable to open image
        std::cerr << "error: image not read from file\n\n";                 // show error message on command line
        return(EXIT_FAILURE);                                               // and exit program
    }

    cv::cvtColor(imgOriginal, imgGrayscale, cv::COLOR_BGR2GRAY );           // Convert to grayscale

    const int gaussian[3][3] = { { 1, 2, 1},                                // Matrix with values of discrete 2D Gaussian
                                 { 2, 4, 2},
                                 { 1, 2, 1} };
    const int sharpen[3][3] =  { {0, -1, 0 },                               // Matrix with values of discrete
                                 {-1, 5, -1},                               // sharpening convolutional filter
                                 {0, -1, 0 } };
    
    cv::Mat outputsharpen;
    cv::Mat outputblurred;
    imgGrayscale.copyTo(outputblurred);                                     // Just to make sure the Mat objects are of the same size. 
    imgGrayscale.copyTo(outputsharpen);

    for (int x = 1; x < imgGrayscale.rows - 1; ++x)                         // Iterate through the image pixels except of the edge ones
    {                                                                       // as we cannot do convolution for them 
        for (int y = 1; y < imgGrayscale.cols - 1; ++y)                     // (missing neighbours partially)
        {
            int core1_summ = 0;
            int core2_summ = 0;
            // Accesssing values of each pixel
            for (int i = -1; i <= 1; ++i)
                for (int j = -1; j <= 1; ++j)
                {
                    core1_summ += imgGrayscale.at<uchar>(x + i, y + j) * gaussian[1 + i][1 + j];
                    core2_summ += imgGrayscale.at<uchar>(x + i, y + j) * sharpen[1 + i][1 + j];
                }
            outputblurred.at<uchar>(x, y) = (core1_summ / 16);              // Normalize the value of smoothed pixel
            if (abs(core2_summ) < 255 and core2_summ > 0)                   // Check if sharpened value is within [0;255] range
                outputsharpen.at<uchar>(x, y) = (core2_summ);               // otherwise copy the original pixel value
            else
                outputsharpen.at<uchar>(x, y) = (imgGrayscale.at<uchar>(x, y));
        }
    }

    std::cout << "Image succesfully processed using convolutional operations.\n";
    //Create windows to show image
    cv::hconcat(outputblurred, outputsharpen, outputblurred);
    cv::imshow("Original", imgGrayscale);
    cv::imshow("Blurred/Sharpened", outputblurred);

    while (cv::waitKey(0) != 27)                                            // Wait for pressing ESC
    {
    }
    return 0;
}