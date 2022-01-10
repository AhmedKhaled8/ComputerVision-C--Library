#include "../../include/filters/median_filter.h"
#include <algorithm>

using cv::Mat;

uchar get_median(uchar* array)
{
  std::sort(array, array + 9);
  return array[4];
}

Mat med_filter(Mat mat)
{
  // accept only char type matrices
  CV_Assert(mat.depth() == CV_8U);

  Mat matrix = mat.clone();

  int channels = matrix.channels();
  int nRows = matrix.rows;
  int nCols = matrix.cols * channels;

  int i,j,k;
  uchar* previous;
  uchar* current;
  uchar* next;

  uchar value = 0;

  for( i = 1; i < nRows - 1; ++i)
  {
    previous = matrix.ptr<uchar>(i - 1);
    current = matrix.ptr<uchar>(i  );
    next = matrix.ptr<uchar>(i+ 1);
    for ( j = 1; j < nCols - 1; j += 3)
    {
      for (k = 0; k < channels; ++k)
      {
        uchar array[] = {previous[j - channels + k] , previous[j + k] , previous[j + channels + k] ,
          current[j - channels + k] , current[j + k] , current[j + channels + k] ,
          next[j - channels + k] , next[j + k] , next[j + channels + k]};
          current[j + k] = get_median(array);

          // std::cout<<"current "<<(int)current[j + k]<<" array "<<(int)array[0]<<std::endl;
      }
    }
  }
  return matrix;
}
