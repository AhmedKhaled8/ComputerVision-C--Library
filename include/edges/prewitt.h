#ifndef _PREWITT_
#define _PREWITT_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class prewitt
{
private:
    Mat img;        //Original Image
    Mat grayscaled; // Grayscale
    Mat gFiltered;  // Gaussian
public:
    prewitt(Mat); //Constructor
    Mat prewittFilter();                                     //Prewitt filtering
    Mat pFiltered;                                         //Prewitt Filtered
    Mat angles;                                            //Angle Map
};

#endif