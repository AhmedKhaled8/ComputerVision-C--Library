#ifndef _GAUSSIAN_
#define _GAUSSIAN_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

class gaussian
{
private:
    Mat img; //Original Image
public:
    gaussian(Mat);                                         //Constructor
    vector<vector<double>> createFilter(int, int, double); //Creates a gaussian filter
    Mat useFilter(Mat, vector<vector<double>>);            //Use some filter
    Mat gFiltered;                                         // Gaussian
};

#endif