#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/snake.h"

using namespace std;
using cv::Mat;

snake::snake(Mat image)
{
    img = image;
    
}

void onMouse1(int event,int x,int y,int foo,void* p)
{
	if (event!= EVENT_LBUTTONDOWN)
		return;
	Point2d m1(x,y);
    Data *D = (Data *)p;
    D->polygon.addPoint(m1);
    // cout << "Adding point: (" << x << ", " << y << ")" << endl;
    D->polygon.drawPolygon(D->image.clone());
}

Mat snake::float2byte(const Mat& If)
{
    double minVal, maxVal;
    minMaxLoc(If,&minVal,&maxVal);
    Mat Ib;
    If.convertTo(Ib, CV_8U, 255.0/(maxVal - minVal),-minVal * 255.0/(maxVal - minVal));
    return Ib;
}

Mat snake::snakeContour()
{
    namedWindow(WIN_NAME);
    imshow(WIN_NAME,img);
    Data *D = new Data(img);
    Mat ContouredImage = D->image.clone();
    Pointxy point(0,0);
    bool modifyPolygon = true;
    float2byte(D->gGradient);
    float2byte(D->gx);
    float2byte(D->gy);
    imshow(WIN_NAME, float2byte(D->image));
    setMouseCallback(WIN_NAME, onMouse1, D);
    waitKey();
    setMouseCallback(WIN_NAME, NULL, NULL);
    Mat Imcloned;
    while (modifyPolygon) {
 	  	D->drawNextStep(STEP, D->image.clone(), mode);
    	waitKey();
    	D->findContour(STEP, mode);
    	//cout << "found contour" << endl;
    	D->polygon.drawPolygon(D->image.clone());
        ContouredImage = D->polygon.drawPolygon(D->image.clone());
    	waitKey();
        if(waitKey() % 256 == 27)
            modifyPolygon = false;
    }
    destroyWindow(WIN_NAME);
    for (int i = 0 ; i<D->polygon.points.size();i++)
    {
        point.x = D->polygon.points[i].x;
        point.y = D->polygon.points[i].y;
        snakePolygon.add_point(point);
    }
    return ContouredImage;
}
