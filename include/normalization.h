#ifndef _NORMALIZATION_
#define _NORMALIZATION_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class normalization
{
public:
    normalization(Mat); // Constructor
    Mat normalize();    // Image Normalization

private:
    Mat img; // Orignal Image
};

#endif // _NORMALIZATION_