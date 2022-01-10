#ifndef kernels_filter_H
#define kernels_filter_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>

using namespace cv;


cv::Mat apply_filter(cv::Mat src, char mode);

cv::Mat get_hybrid(cv::Mat lpf_src, cv::Mat hpf_src);
#endif
