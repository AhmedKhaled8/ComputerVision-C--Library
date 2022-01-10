#include "../../include/noise/add_noise.h"
#include <random>

using cv::Mat;


Mat gauss_noise(Mat matrix)
{
  // accept only char type matrices
  CV_Assert(matrix.depth() == CV_8U);

  // uniformly-distributed integer random number generator
  std::random_device rd{};
  // random number generator produces high quality unsigned integer random numbers
  std::mt19937 gen{rd()};
  // create a uniform distribution with mean 0 and standard deviation 1
  std::normal_distribution<float> distribution{0,20};

  int channels = matrix.channels();
  int nRows = matrix.rows;
  int nCols = matrix.cols * channels;

  if (matrix.isContinuous())
  {
    std::cout<<"Continuous matrix"<<std::endl;
    nCols = nCols * nRows;
    nRows = 1;
  }

  float added_noise = 0.0f;
  int i,j,k;
  uchar* p;

  for( i = 0; i < nRows; ++i)
  {
    p = matrix.ptr<uchar>(i);
    for ( j = 0; j < nCols; j +=3)
    {
      added_noise = distribution(gen);
      for (k = 0; k < channels; ++k)
      {
        p[j+k] += (uchar)added_noise;
      }
    }
  }

  return matrix;
}
