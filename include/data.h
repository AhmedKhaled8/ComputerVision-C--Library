#pragma once
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std; 

#include "../include/polygon.h"
enum SegmentationMode {
	BALLOON_MODE,
	CURV_MODE,
	ALL_MODE
};

class Data {

public: 
  Data(Mat &Image);
  Mat image;
  Mat bwImage; 
  Mat gradient; 
  Mat gGradient; 
  Mat gx; 
  Mat gy; 
  Polygon polygon; 

  void drawNextStep(double step, const Mat &Image, SegmentationMode mode);
  void findContour(double step, SegmentationMode mode);
  bool isValidPoint(Point2d p);
};