#ifndef FFT_Filter_H
#define FFT_Filter_H

#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include <cmath>

#define REAL 0
#define IMG 1

using namespace std;
int *zigZagMatrix(int** arr, int n, int m);
int *create_zigzag(int rows, int columns);
bool check_low_freq(int *zigzag, int freq, int size);
cv::Mat fft_filter(cv::Mat src, char mode);

#endif