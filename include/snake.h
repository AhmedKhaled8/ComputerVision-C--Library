#ifndef _SNAKE_
#define _SNAKE_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "polygon.h"
#include "data.h"
#include "contour.h"

using namespace std;
using cv::Mat;

class snake
{
private:
    Mat img;        //Original Image
    Mat float2byte(const Mat& If);
    const float STEP = 5;
    const SegmentationMode mode = ALL_MODE;
    const string WIN_NAME = "Please Define Polygon";

public:
    snake(Mat); //Constructor
    Contour snakePolygon;
    Mat snakeContour();
};

#endif
