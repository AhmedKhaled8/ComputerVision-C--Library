#ifndef HOUGH_H
#define HOUGH_H

#include <opencv2/imgproc/imgproc.hpp>
using cv::Mat;

Mat hough_circle(Mat _img, uint threshold = 65, uint num_division = 10, uint num_theta = 100);

#endif //HOUGH_H
