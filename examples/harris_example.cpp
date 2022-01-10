#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../include/harris.h"

int main() {

    // box.jpg ==> 2e7
    // chess.png ==> 1e7
    Harris h;
    cv::Mat img = cv::imread("chess.png");
    cv::Mat dst = harrisCornerDetection(img, 12, 1e7, h);
    cv::imshow("output", dst);
    cv::waitKey(0);
    //    cv::Mat img = cv::imread("chess.png", cv::IMREAD_GRAYSCALE);
//    cv::Mat imgBlurred;
//    cv::GaussianBlur(img, imgBlurred, cv::Size(9, 9), 0.5);
//    cv::Mat rgb = cv::imread("chess.png");
//    cv::Mat padded = addPadding(imgBlurred, 16);
//    Harris h = applyHarris(padded, 1e7);
//    drawCorners(rgb, h.points, 16);
    return 0;
}
