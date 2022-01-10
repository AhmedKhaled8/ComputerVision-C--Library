#include <iostream>
#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include <cmath>

using namespace cv;
using namespace std;

#define REAL 0
#define IMG 1

char pass_factor;



int *zigZagMatrix(int** arr, int n, int m)
{
    int *zigzag = new int[m*n];
    int index = 0;
    int row = 0, col = 0;
  
    // Boolean variable that will true if we
    // need to increment 'row' value otherwise
    // false- if increment 'col' value
    bool row_inc = 0;
  
    // Print matrix of lower half zig-zag pattern
    int mn = min(m, n);
    for (int len = 1; len <= mn; ++len) {
        for (int i = 0; i < len; ++i) {
            // cout << arr[row][col] << " ";
            zigzag[index] = arr[row][col];
            index++;
  
            if (i + 1 == len)
                break;
            // If row_increment value is true
            // increment row and decrement col
            // else decrement row and increment
            // col
            if (row_inc)
                ++row, --col;
            else
                --row, ++col;
        }
  
        if (len == mn)
            break;
  
        // Update row or col value according
        // to the last increment
        if (row_inc)
            ++row, row_inc = false;
        else
            ++col, row_inc = true;
    }
  
    // Update the indexes of row and col variable
    if (row == 0) {
        if (col == m - 1)
            ++row;
        else
            ++col;
        row_inc = 1;
    }
    else {
        if (row == n - 1)
            ++col;
        else
            ++row;
        row_inc = 0;
    }
  
    // Print the next half zig-zag pattern
    int MAX = max(m, n) - 1;
    for (int len, diag = MAX; diag > 0; --diag) {
  
        if (diag > mn)
            len = mn;
        else
            len = diag;
  
        for (int i = 0; i < len; ++i) {
            // cout << arr[row][col] << " ";
            zigzag[index] = arr[row][col];
            index++;
  
            if (i + 1 == len)
                break;
  
            // Update row or col value according
            // to the last increment
            if (row_inc)
                ++row, --col;
            else
                ++col, --row;
        }
  
        // Update the indexes of row and col variable
        if (row == 0 || col == m - 1) {
            if (col == m - 1)
                ++row;
            else
                ++col;
  
            row_inc = true;
        }
  
        else if (col == 0 || row == n - 1) {
            if (row == n - 1)
                ++col;
            else
                ++row;
  
            row_inc = false;
        }
    }
    return zigzag;
}

int *create_zigzag(int rows, int columns)
{
    int ** arr = new int*[rows];

    for(int i = 0; i < rows; i++)
    {
        arr[i] = new int[columns];
    }

    int ctr = 1;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            arr[i][j] = ctr;
            ctr++;
        }
    }
    
    int* solution = zigZagMatrix(arr, rows, columns);

    for(int i = 0; i < rows; i++)
    {
        delete arr[i];
    }
    
    delete arr;

    return solution;


}

bool check_low_freq(int *zigzag, int freq, double threshold, double max)
{
    bool low_freq = false;
    for(int i=3000; i < 10000; i++)
    {
        if(zigzag[i] == freq)
        {
            low_freq = true;
            break;
        }
    }
    return low_freq;
}

int main(){
    
    // READ IMAGE  
    Mat src;
    src = imread("lenna.jpeg", 0);

    int rows = src.rows;
    int cols = src.cols;

    double samp_freq = rows * cols;

    double high_thresh = samp_freq / 6 * 5;

    cout << rows << cols << endl;
    float fft_factor = sqrt(rows * cols);

    if(!src.data)
    {
        cout << "Image not available" << endl;
        return -1;
    }

    // define fftw instances
    fftw_complex *img_spatial;
    fftw_complex *img_freq;
    fftw_complex *img_inverse;


    // malloc fftw instances
    img_spatial = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * rows * cols);
    img_freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * rows * cols);
    img_inverse = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * rows * cols);
    fftw_plan forward_fft = fftw_plan_dft_2d(rows, cols, img_spatial, img_freq, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan inverse_fft = fftw_plan_dft_2d(rows, cols, img_freq, img_inverse, FFTW_BACKWARD, FFTW_ESTIMATE);

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            img_spatial[c + cols * r][REAL] = src.at<uchar>(r, c);
            img_spatial[c + cols * r][IMG] = 0;
        }
    }

    fftw_execute(forward_fft);


    int *zigzag = create_zigzag(rows, cols);

    // apply freq filter
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            if(check_low_freq(zigzag, c + r * cols + 1, high_thresh, samp_freq))
            {
                pass_factor = 1;
            }
            else
            {
                pass_factor = 0;
            }
            img_freq[c + cols * r][0] *= (pass_factor / fft_factor);
            img_freq[c + cols * r][1] *= (pass_factor / fft_factor);            
        }
    }

    // ifft

    fftw_execute(inverse_fft);


    Mat dst = src.clone();
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            dst.at<uchar>(r, c) = (int) (img_inverse[c + cols * r][0]  / fft_factor);
        }
    }

    fftw_destroy_plan(forward_fft);
    fftw_destroy_plan(inverse_fft);
    fftw_free(img_spatial);
    fftw_free(img_freq);

    namedWindow("initial");
    imshow("intial", src);
    waitKey();

    namedWindow("result");
    imshow("result", dst);
    
    waitKey();

    delete zigzag;
    return 0;


}