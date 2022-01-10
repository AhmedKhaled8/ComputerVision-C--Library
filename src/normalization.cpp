#include "../include/normalization.h"
#include <iostream>
#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

normalization::normalization(Mat original_image)
{
    img = original_image;
}

Mat normalization::normalize()
{
    int original_img_height = img.rows;
    int original_img_width = img.cols;
    int min_value = 255;
    int max_value = 0;
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<uchar>(i, j) > max_value)
                max_value = img.at<uchar>(i, j);
            if (img.at<uchar>(i, j) < min_value)
                min_value = img.at<uchar>(i, j);
        }
    Mat normalized_img(original_img_height, original_img_width, CV_8UC1); // Create an Empty Image with Same Height and Width of Original Image
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            normalized_img.at<uchar>(i, j) = ((int)img.at<uchar>(i, j) - min_value) * 255 / (max_value - min_value); // Grayscale Image Only One Channel
        }
    return normalized_img;
}