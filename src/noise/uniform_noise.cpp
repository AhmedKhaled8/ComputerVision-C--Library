#include "../../include/noise/add_noise.h"

using cv::Mat;

int gen_uniform_neg(int& rand_range)
{
  // rand() generates a uniform random variable
  // dividing yeald a number between 0 and RAND_MAX/rand_range
  // subtracting 128 makes the value between -128 and +127 for positive and negative noise
  return (rand()/ rand_range - (RAND_MAX/rand_range/2));
}

Mat uniform_noise(Mat matrix)
{
  // accept only char type matrices
  CV_Assert(matrix.depth() == CV_8U);

  // Mat matrix = mat.clone();

  int channels = matrix.channels();
  int nRows = matrix.rows;
  int nCols = matrix.cols * channels;

  if (matrix.isContinuous())
  {
    std::cout<<"Continuous matrix"<<std::endl;
    nCols = nCols * nRows;
    nRows = 1;
  }
  int added_noise = 0;
  int dividing_num = RAND_MAX/20;
  int i,j,k;
  uchar* p;

  for( i = 0; i < nRows; ++i)
  {
    p = matrix.ptr<uchar>(i);
    for ( j = 0; j < nCols; j += 3)
    {
      added_noise = gen_uniform_neg(dividing_num);
      for (k = 0; k < channels; ++k)
      {
        p[j+k] += added_noise;
      }
    }
  }
  return matrix;
}
