#ifndef add_noise_H // include guard
#define add_noise_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

cv::Mat uniform_noise(cv::Mat);
cv::Mat gauss_noise(cv::Mat);
cv::Mat salt_pepper_noise(cv::Mat);

#endif
