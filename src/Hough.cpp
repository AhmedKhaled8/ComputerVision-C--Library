#include "../include/Hough.h"

#include <math.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using cv::Mat;

typedef unsigned int uint;
typedef short unsigned int ushort;

void linspace(float start, float end, unsigned int num, float *output_array)
{
  // returns an array that with the end number NOT included
  float delta = end - start;
  float step = delta / num;
  for (int i = 0; i < num; ++i)
  {
    output_array[i] = step * i + start;
  }
}

void linspace(float start, float end, unsigned int num, ushort *output_array)
{
  // returns an array that with the end number NOT included
  float delta = end - start;
  float step = delta / num;
  for (int i = 0; i < num; ++i)
  {
    output_array[i] = step * i + start;
  }
}

Mat hough_circle(Mat _img, uint threshold, uint num_division, uint num_theta)
{
  // initialize indexer
  uint idx1 = 0;
  uint idx2 = 0;
  // img holds the canny result of original image
  Mat img;
  // The returned image with overlay
  Mat color_photo;
  // Apply canny to original photo
  cv::Canny(_img, img, 50, 80);

  // If the photo is grayscale convert to BGR
  if (_img.channels() == 1)
  {cv::cvtColor(_img, color_photo, cv::COLOR_GRAY2BGR);}
  // else clone original image to color_photo
  else {color_photo = _img.clone();}

  float pi = 3.14159265;
  // Array to hold angle values
  float thetas [num_theta];
  // initialize angles array
  linspace(0, 360, num_theta, thetas);
  // Convert from degree to radian
  for (idx1 = 0; idx1 < num_theta; ++idx1)
  {thetas[idx1] = thetas[idx1] * pi / 180.0f;}

  ushort height = img.rows;
  ushort width = img.cols;

  // the diagonal length is the square root of W^2 + H^2
  ushort diag_len = sqrt(width * width + height * height) + 1;
  // Assign the minimum dimension
  ushort min_dim = width < height ? width : height;
  // Get the number of rhos
  ushort num_rhos = min_dim / num_division;

  // Allocate array with number of rhos
  ushort rhos[num_rhos];
  // initialize rhos
  linspace(10,(int) (min_dim / 2), num_rhos, rhos);

  // Allocate array that will hold cosine values of thetas
  float cos_array[num_theta];
  // Allocate array that will hold sine values of thetas
  float sin_array[num_theta];
  // Assign values of cosine and sine
  for (idx1 = 0; idx1 < num_theta; ++idx1)
  {
    cos_array[idx1] = cos(thetas[idx1]);
    sin_array[idx1] = sin(thetas[idx1]);
    // printf("Cos: %f, Sin: %f\n", cos_array[idx1], sin_array[idx1]);
  }

  // Get the accumulator size
  ushort acc_size = 2 * diag_len + 1;
  // Allocate the accumulator and initialize to zero
  ushort accumulator[acc_size][acc_size] = {};

  uchar* row;
  ushort x = 0;
  ushort y = 0;
  ushort x_acc = 0;
  ushort y_acc = 0;
  int x_val = 0;
  int y_val = 0;
  ushort theta_idx = 0;
  ushort a;
  ushort b;
  // ushort counter = 0;
  float rhoMULcos[num_theta];
  float rhoMULsin[num_theta];

  for (idx1 = 0; idx1 < num_rhos; ++idx1)
  {
    for (idx2 = 0; idx2 < num_theta; ++idx2)
    {
      rhoMULcos[idx2] = rhos[idx1] * cos_array[idx2];
      rhoMULsin[idx2] = rhos[idx1] * sin_array[idx2];
    }
    // printf("%d\n", rhos[idx1]);
    for (y = 0; y < height; ++y)
    {
      row = img.ptr<uchar>(y);
      for(x = 0; x < width; ++x)
      {
        if (row[x] !=0)
        {
          for (theta_idx = 0; theta_idx < num_theta; ++theta_idx)
          {
            a = x - rhoMULcos[theta_idx] + diag_len;
            b = y - rhoMULsin[theta_idx] + diag_len;
            if (a >= acc_size or b >= acc_size) {continue;}
            // printf("Acc is in range\n");
            // b == 406 ? printf("y: %d\n", y): false;
            // printf("b is %d\n", b);
            accumulator[a][b] += 1;
            // printf("Acc is %d", accumulator[a][b]);
          }
        }
      }
    }
    for (y_acc = 0; y_acc < acc_size; ++y_acc)
    {
      // row = color_photo.ptr<uchar>(5);
      for (x_acc = 0; x_acc < acc_size; ++x_acc)
      {
        // counter +=1;
        // printf("y value: %d\n", y_acc);
        // if (accumulator[x_acc][y_acc] > 0) {printf("accumulator %d\n" ,accumulator[x_acc][y_acc]);}
        if(accumulator[x_acc][y_acc] < threshold){continue;}
        for (uint theta_idx2 = 0; theta_idx2 < num_theta; ++theta_idx2)
        {
          // printf("Acc\n");
          x_val = x_acc - diag_len + rhoMULcos[theta_idx2];
          // printf("x acc: %d, x val: %d, theta: %f\n", x_acc, x_val, rhoMULcos[theta_idx2]);
          // printf("x val: %d, ", x_val);
          // printf("theta: %f\n", rhoMULcos[theta_idx2]);
          y_val = y_acc - diag_len + rhoMULsin[theta_idx2];
          // printf("y val: %d\n", y_val);
          // printf("y acc: %d\n", y_acc);
          // printf("theta: %f\n", rhoMULsin[theta_idx2]);
          if (x_val < 0 || y_val < 0) {continue;}
          if (x_val >= width || y_val >= height) {continue;}
          // printf("y acc: %d, y val: %d, theta: %f\n", y_acc, y_val, rhoMULcos[theta_idx2]);
          row = color_photo.ptr<uchar>(y_val);
          row[3 * x_val+0] = 255;
          row[3 * x_val+1] = 0;
          row[3 * x_val+2] = 0;
        }
      }
    }
  }
  // printf("Acc at position: %d", accumulator[231][195]);
  // printf("Acc at position: %d\n", accumulator[195][231]);

  // printf("Counter %d\n", counter);
  // printf("Width %d\n", width);
  // printf("Height %d\n", height);

  return color_photo;

}
