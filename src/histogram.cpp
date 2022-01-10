#include "../include/histogram.h"
#include <iostream>

#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

histogram::histogram(Mat original_image)
{
    img = original_image;
}

void histogram::computeHistogram(String color)
{
    // initialize all intensity values to 0
    for (int i = 0; i < 256; i++)
    {
        histo[i] = 0;
    }

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            if (img.channels() == 3)
            {
                int b = img.at<Vec3b>(i, j)[0];
                blueArray.push_back(b);
                int g = img.at<Vec3b>(i, j)[1];
                greenArray.push_back(g);
                int r = img.at<Vec3b>(i, j)[2];
                redArray.push_back(r);
            }
            else if (img.channels() == 1)
            {
                int grey = img.at<Vec3b>(i, j)[0];
                greyscaleArray.push_back(grey);
            }
        }
    // calculate the no of pixels for each intensity values

    if (color == "red")
        for (int x = 0; x < redArray.size(); x++)
            histo[(int)redArray[x]]++;
    else if (color == "green")
        for (int x = 0; x < greenArray.size(); x++)
            histo[(int)greenArray[x]]++;
    else if (color == "blue")
        for (int x = 0; x < blueArray.size(); x++)
            histo[(int)blueArray[x]]++;
    else if (color == "greyscale")
        for (int x = 0; x < greyscaleArray.size(); x++)
            histo[(int)greyscaleArray[x]]++;

    histDisplay(histo, color, "histogram");
}

void histogram::computeCumulative(String color)
{
    computeHistogram(color);
    int cum[256];
    for (int i = 0; i < 256; ++i)
    {
        if (i != 0)
            cum[i] = cum[i - 1] + histo[i];
        else
            cum[i] = histo[i];
    }

    histDisplay(cum, color, "cumulative");
}

void histogram::histDisplay(int histogram[], String name, String type)
{
    int hist[256];
    for (int i = 0; i < 256; i++)
    {
        hist[i] = histogram[i];
    }
    // draw the histograms
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double)hist_w / 256);

    Mat histImg(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
    histImage = histImg;
    cumImage = histImg;

    // find the maximum intensity element from histogram
    int max = hist[0];
    for (int i = 1; i < 256; i++)
    {
        if (max < hist[i])
        {
            max = hist[i];
        }
    }

    // normalize the histogram between 0 and histImage.rows
    for (int i = 0; i < 256; i++)
    {
        hist[i] = ((double)hist[i] / max) * histImage.rows;
    }
    Scalar color;
    if (name == "red")
        color = Scalar(0, 0, 255);
    else if (name == "green")
        color = Scalar(0, 255, 0);
    else if (name == "blue")
        color = Scalar(255, 0, 0);
    else if (name == "greyscale")
        color = Scalar(0, 0, 0);
    // draw the intensity line for histogram
    for (int j = 0; j < 256; j++)
    {
        if (type == "histogram")
            line(histImage, Point(bin_w * (j), hist_h), Point(bin_w * (j), hist_h - hist[j]), color, 1, 8, 0);
        else if (type == "cumulative")
            line(cumImage, Point(bin_w * (j), hist_h), Point(bin_w * (j), hist_h - hist[j]), color, 1, 8, 0);
    }
}
