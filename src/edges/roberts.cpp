#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../../include/greyscale.h"
#include "../../include/filters/gaussian_filter.h"
#include "../../include/edges/roberts.h"

using namespace std;
using namespace cv;

roberts::roberts(Mat image)
{
    img = image;
    gaussian gauss(img);
    greyscale grey(img);

    vector<vector<double>> filter = gauss.createFilter(3, 3, 1);

    grayscaled = Mat(grey.toGrayScale());                //Grayscale the image
    gFiltered = Mat(gauss.useFilter(grayscaled, filter)); //Gaussian Filter
    rFiltered = Mat(robertsFilter()); //Roberts Filter
}

Mat roberts::robertsFilter()
{

    //Roberts X Filter
    double x1[] = {1.0, 0};
    double x2[] = {0, -1.0};

    vector<vector<double>> xFilter(2);
    xFilter[0].assign(x1, x1 + 2);
    xFilter[1].assign(x2, x2 + 2);

    //Roberts Y Filter
    double y1[] = {0, 1.0};
    double y2[] = {-1.0, 0};

    vector<vector<double>> yFilter(2);
    yFilter[0].assign(y1, y1 + 2);
    yFilter[1].assign(y2, y2 + 2);

    //Limit Size
    int size = (int)xFilter.size() / 2;

    Mat filteredImg = Mat(gFiltered.rows - 2 * size, gFiltered.cols - 2 * size, CV_8UC1);

    angles = Mat(gFiltered.rows - 2 * size, gFiltered.cols - 2 * size, CV_32FC1); //AngleMap

    for (int i = size; i < gFiltered.rows - size; i++)
    {
        for (int j = size; j < gFiltered.cols - size; j++)
        {
            double sumx = 0;
            double sumy = 0;

            for (int x = 0; x < xFilter.size(); x++)
                for (int y = 0; y < xFilter.size(); y++)
                {
                    sumx += xFilter[x][y] * (double)(gFiltered.at<uchar>(i + x - size, j + y - size)); //Roberts_X Filter Value
                    sumy += yFilter[x][y] * (double)(gFiltered.at<uchar>(i + x - size, j + y - size)); //Roberts_Y Filter Value
                }
            double sumxsq = sumx * sumx;
            double sumysq = sumy * sumy;

            double sq2 = sqrt(sumxsq + sumysq);

            if (sq2 > 255) //Unsigned Char Fix
                sq2 = 255;
            filteredImg.at<uchar>(i - size, j - size) = sq2;

            if (sumx == 0) //Arctan Fix
                angles.at<float>(i - size, j - size) = 90;
            else
                angles.at<float>(i - size, j - size) = atan(sumy / sumx);
        }
    }

    return filteredImg;
}