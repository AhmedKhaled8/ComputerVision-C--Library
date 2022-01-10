#include "../../include/noise/add_noise.h"

using cv::Mat;

int gen_uniform(int& rand_range)
{
  // rand() generates a uniform random variable
  // dividing yeald a number between 0 and RAND_MAX/rand_range
  return (rand()/ rand_range);
}

Mat salt_pepper_noise(Mat matrix)
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
  int probability = 5;
  int threshold = 100 - probability;
  int random_num = 0;
  int dividing_num = RAND_MAX/100;
  int i,j,k;
  uchar* p;

  for( i = 0; i < nRows; ++i)
  {
    p = matrix.ptr<uchar>(i);
    for ( j = 0; j < nCols; j +=3)
    {
      random_num = gen_uniform(dividing_num);
      for (k = 0; k < channels; ++k)
      {
        if (random_num < probability) {p[j+k] = 0;}
        else if (random_num > threshold) {p[j+k] = 255;}
      }
    }
  }
  return matrix;
}
