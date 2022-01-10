#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../../include/filters/gaussian_filter.h"

using namespace std;
using namespace cv;

gaussian::gaussian(Mat image)
{
    img = image;

    vector<vector<double>> filter = createFilter(3, 3, 1);
    gFiltered = Mat(useFilter(img, filter)); //Gaussian Filter
}

vector<vector<double>> gaussian::createFilter(int row, int column, double sigmaIn)
{
    vector<vector<double>> filter;

    for (int i = 0; i < row; i++)
    {
        vector<double> col;
        for (int j = 0; j < column; j++)
        {
            col.push_back(-1);
        }
        filter.push_back(col);
    }

    float coordSum = 0;
    float constant = 2.0 * sigmaIn * sigmaIn;

    // Sum is for normalization
    float sum = 0.0;

    for (int x = -row / 2; x <= row / 2; x++)
    {
        for (int y = -column / 2; y <= column / 2; y++)
        {
            coordSum = (x * x + y * y);
            filter[x + row / 2][y + column / 2] = (exp(-(coordSum) / constant)) / (M_PI * constant);
            sum += filter[x + row / 2][y + column / 2];
        }
    }

    // Normalize the Filter
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            filter[i][j] /= sum;

    return filter;
}

Mat gaussian::useFilter(Mat img_in, vector<vector<double>> filterIn)
{
    int size = (int)filterIn.size() / 2;
    Mat filteredImg = Mat(img_in.rows - 2 * size, img_in.cols - 2 * size, CV_8UC3);
    for (int i = size; i < img_in.rows - size; i++)
    {
        for (int j = size; j < img_in.cols - size; j++)
        {

            double sumX = 0;
            double sumY = 0;
            double sumZ = 0;

            for (int x = 0; x < filterIn.size(); x++)
                for (int y = 0; y < filterIn.size(); y++)
                {
                    sumX += filterIn[x][y] * (double)img_in.at<Point3_<uchar>>(i + x - size, j + y - size).x;
                    sumY += filterIn[x][y] * (double)img_in.at<Point3_<uchar>>(i + x - size, j + y - size).y;
                    sumZ += filterIn[x][y] * (double)img_in.at<Point3_<uchar>>(i + x - size, j + y - size).z;
                }

            filteredImg.at<Point3_<uchar>>(i - size, j - size).x = sumX;
            filteredImg.at<Point3_<uchar>>(i - size, j - size).y = sumY;
            filteredImg.at<Point3_<uchar>>(i - size, j - size).z = sumZ;
        }
    }
    return filteredImg;
}
