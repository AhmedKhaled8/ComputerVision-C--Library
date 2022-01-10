#ifndef _EQUALIZATION_
#define _EQUALIZATION_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class equalization
{
public:
    equalization(Mat); // Constructor
    Mat equalize();    // Image Equalization

private:
    Mat img; // Orignal Image
};

#endif // _EQUALIZATION_