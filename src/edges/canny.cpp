#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../../include/edges/sobel.h"
#include "../../include/edges/canny.h"

using namespace std;
using namespace cv;

canny::canny(Mat image)
{
    img = image;
    sobel sbl(img);
    angles = Mat(sbl.angles); //Sobel Filter
    sFiltered = Mat(sbl.sFiltered);      //Angle Map
    non = Mat(nonMaxSupp());             //Non-Maxima Suppression
    thres = Mat(threshold(non, 30, 80)); //Double Threshold and Finalize
}

Mat canny::nonMaxSupp()
{
    Mat nonMaxSupped = Mat(sFiltered.rows - 2, sFiltered.cols - 2, CV_8UC1);
    for (int i = 1; i < sFiltered.rows - 1; i++)
    {
        for (int j = 1; j < sFiltered.cols - 1; j++)
        {
            float Tangent = angles.at<float>(i, j);

            nonMaxSupped.at<uchar>(i - 1, j - 1) = sFiltered.at<uchar>(i, j);
            //Horizontal Edge
            if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }
            //Vertical Edge
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            //-45 Degree Edge
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            //45 Degree Edge
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }
        }
    }
    return nonMaxSupped;
}

Mat canny::threshold(Mat imgin, int low, int high)
{
    if (low > 255)
        low = 255;
    if (high > 255)
        high = 255;

    Mat EdgeMat = Mat(imgin.rows, imgin.cols, imgin.type());

    for (int i = 0; i < imgin.rows; i++)
    {
        for (int j = 0; j < imgin.cols; j++)
        {
            EdgeMat.at<uchar>(i, j) = imgin.at<uchar>(i, j);
            if (EdgeMat.at<uchar>(i, j) > high)
                EdgeMat.at<uchar>(i, j) = 255;
            else if (EdgeMat.at<uchar>(i, j) < low)
                EdgeMat.at<uchar>(i, j) = 0;
            else
            {
                bool anyHigh = false;
                bool anyBetween = false;
                for (int x = i - 1; x < i + 2; x++)
                {
                    for (int y = j - 1; y < j + 2; y++)
                    {
                        if (x <= 0 || y <= 0 || EdgeMat.rows || y > EdgeMat.cols) //Out of bounds
                            continue;
                        else
                        {
                            if (EdgeMat.at<uchar>(x, y) > high)
                            {
                                EdgeMat.at<uchar>(i, j) = 255;
                                anyHigh = true;
                                break;
                            }
                            else if (EdgeMat.at<uchar>(x, y) <= high && EdgeMat.at<uchar>(x, y) >= low)
                                anyBetween = true;
                        }
                    }
                    if (anyHigh)
                        break;
                }
                if (!anyHigh && anyBetween)
                    for (int x = i - 2; x < i + 3; x++)
                    {
                        for (int y = j - 1; y < j + 3; y++)
                        {
                            if (x < 0 || y < 0 || x > EdgeMat.rows || y > EdgeMat.cols) //Out of bounds
                                continue;
                            else
                            {
                                if (EdgeMat.at<uchar>(x, y) > high)
                                {
                                    EdgeMat.at<uchar>(i, j) = 255;
                                    anyHigh = true;
                                    break;
                                }
                            }
                        }
                        if (anyHigh)
                            break;
                    }
                if (!anyHigh)
                    EdgeMat.at<uchar>(i, j) = 0;
            }
        }
    }
    return EdgeMat;
}
