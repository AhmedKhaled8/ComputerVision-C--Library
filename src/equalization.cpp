#include "../include/equalization.h"
#include <iostream>
#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

equalization::equalization(Mat original_image)
{
    img = original_image;
}

Mat equalization::equalize()
{
    Mat equalizedImage = img.clone();

    int *hist = new int[256] {};
    int *lut = new int[256] {};

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            hist[(int) img.at<uchar>(i, j)]++;

    auto i = 0;
    while (!hist[i])
        ++i;

    auto total = (int) img.total();
    if (hist[i] == total)
    {
        equalizedImage.setTo(i);
        return equalizedImage;
    }

    float scale = (256 - 1.f) / (total - hist[i]);
    auto sum = 0;

    for (lut[i++] = 0; i < 256; ++i)
    {
        sum += hist[i];
        lut[i] = cv::saturate_cast<uchar>(sum * scale);
    }

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            equalizedImage.at<uchar>(i, j) = lut[(int) img.at<uchar>(i, j)];
        }
    return equalizedImage;
}