#ifndef _CANNY_
#define _CANNY_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class canny
{
private:
    Mat img;    //Original Image
    Mat angles; //Angle Map
    Mat sFiltered; //Sobel Filtered
    Mat non;       // Non-maxima supp.
public:
    canny(Mat);                   //Constructor
    Mat nonMaxSupp();             //Non-maxima supp.
    Mat threshold(Mat, int, int); //Double threshold and finalize picture
    Mat thres;                    //Double threshold and final
};

#endif