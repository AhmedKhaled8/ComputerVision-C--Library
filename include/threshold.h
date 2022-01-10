#ifndef _THRESHOLD_
#define _THRESHOLD_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using cv::Mat;
using std::cout, std::endl;
using std::vector, std::string;

class thresh
{
private:
    Mat img; //Original Image
    int threshold;

public:
    thresh(Mat);    //Constructor
    Mat grayscaled; // Grayscale
    Mat globalThreshold(Mat);
    // Mat localThreshold();
    Mat localThreshold(int);
    void optimalThreshold(Mat);
    void otsuThreshold(Mat);
    Mat spectralThreshold(Mat);
    string thresholdMode;
    Mat thresholdImage;
};

#endif