#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
#include "../include/segmentation/kmeans.h"

int main() {
    cv::Mat image = cv::imread("frog.png");
    KMeans_result kmeans = apply_kmeans(image, 3, 20);
    cv::imshow("king", kmeans.segmented_image);
    cv::waitKey(0);
}
