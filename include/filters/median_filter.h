#ifndef med_filter_H // include guard
#define med_filter_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

cv::Mat med_filter(cv::Mat);
uchar get_median(uchar*);

#endif
