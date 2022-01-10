#ifndef _SOBEL_
#define _SOBEL_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class sobel
{
private:
    Mat img;        //Original Image
    Mat grayscaled; // Grayscale
    Mat gFiltered;  // Gaussian
public:
    sobel(Mat); //Constructor
    Mat sobelFilter();                                     //Sobel filtering
    Mat sFiltered;                                         //Sobel Filtered
    Mat angles;                                            //Angle Map
};

#endif