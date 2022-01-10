#ifndef _HISTOGRAM_
#define _HISTOGRAM_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class histogram
{
public:
    histogram(Mat);                // Constructor
    void computeHistogram(String); // Compute Histogram
    void computeCumulative(String);   // Compute Cumulative Histogram
    void histDisplay(int[], String, String);
    Mat histImage;
    Mat cumImage;

private:
    Mat img; // Orignal Image
    int histo[256];
    vector<int> redArray;
    vector<int> greenArray;
    vector<int> blueArray;
    vector<int> greyscaleArray;
};

#endif // _HISTOGRAM_