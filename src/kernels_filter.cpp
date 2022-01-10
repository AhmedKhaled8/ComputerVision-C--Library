#include "../include/kernels_filter.h"


#define kernel_size 3

float lpf_kernel[kernel_size][kernel_size] = {
    {1/32.0, 4/32.0, 1/32.0},
    {4/32.0, 12/32.0, 4/32.0},
    {1/32.0, 4/32.0, 1/32.0}
};

float hpf_kernel[kernel_size][kernel_size] = {
    {0.17, 0.67, 0.17},
    {0.67, -3.33, 0.67},
    {0.17, 0.67, 0.17}

};



cv::Mat apply_filter(cv::Mat src, char mode)
{
    float filter[kernel_size][kernel_size];
    if(mode == 'l')
    {
        for(char i = 0; i < kernel_size; i++)
        {
            for(char j = 0; j < kernel_size; j++)
            {
                filter[i][j] =  lpf_kernel[i][j];
            }
        }
    }
    else if(mode == 'h')
    {
        for(char i = 0; i < kernel_size; i++)
        {
            for(char j = 0; j < kernel_size; j++)
            {
                filter[i][j] =  hpf_kernel[i][j];
            }
        }
    }
    cv::Mat dst = src.clone();
    int rows = src.rows;
    int cols = src.cols;
    float sum;
    for(int r  = 1; r < rows - 1; r++)
    {
        for(int c = 1; c < cols - 1; c++)
        {
            sum = 0.0;
            for(char k = -1; k <= 1; k++)
            {
                for(char j = -1; j <=1 ; j++)
                {
                    sum = sum + filter[j+1][k+1]  * src.at<uchar>(r+j, c+k);
                }
            }
            dst.at<uchar>(r, c) = sum;
        }
    }
    return dst;
}

cv::Mat get_hybrid(cv::Mat lpf_src, cv::Mat hpf_src)
{
  if(lpf_src.channels() > 1)
  {
    cv::cvtColor(lpf_src, lpf_src, cv::COLOR_BGR2GRAY);
  }
  if(hpf_src.channels() > 1)
  {
    cv::cvtColor(hpf_src, hpf_src, cv::COLOR_BGR2GRAY);
  }
    int lpf_rows = lpf_src.rows;
    int lpf_cols = lpf_src.cols;
    int hpf_rows = hpf_src.rows;
    int hpf_cols = hpf_src.cols;
    int min_rows = std::min(lpf_rows, hpf_rows);
    int min_cols = std::min(lpf_cols, hpf_cols);

    // cv::Mat result(min_cols, min_rows, CV_8UC1, Scalar(0));



    cv::Mat lpf_filtered = apply_filter(lpf_src, 'l');
    cv::Mat hpf_filtered = apply_filter(hpf_src, 'h');

    cv::Mat result;
    if(lpf_cols > hpf_cols)
    {
        result = lpf_filtered.clone();
    }
    else
    {
        result = hpf_filtered.clone();
    }

    float val;

    for(int r = 0; r < min_rows; r++)
    {
        for(int c = 0; c < min_cols; c++)
        {
            val = lpf_filtered.at<uchar>(r, c) + hpf_filtered.at<uchar>(r, c);

            if(val < 0) val = 0;
            else if(val > 255) val = 255;
            result.at<uchar>(r, c) = val;
        }
    }

    return result;
}
