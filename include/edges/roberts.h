#ifndef _ROBERTS_
#define _ROBERTS_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class roberts
{
private:
    Mat img;        //Original Image
    Mat grayscaled; // Grayscale
    Mat gFiltered;  // Gaussian
public:
    roberts(Mat); //Constructor
    Mat robertsFilter();                                     //Roberts filtering
    Mat rFiltered;                                         //Roberts Filtered
    Mat angles;                                            //Angle Map
};

#endif