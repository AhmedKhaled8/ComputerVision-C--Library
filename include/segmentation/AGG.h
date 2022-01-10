#ifndef AGG_H
#define AGG_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class AGG
{
public:
    AGG(Mat);           /* Constructor */
    //vector<float> euclidean_distance()
    vector<double> InitialClusters();


private:
    Mat OriginalImg;    /* Orignal Image */
    Mat ResultImg;      /* Result Image (After AGG) */
};

#endif // AGG_H