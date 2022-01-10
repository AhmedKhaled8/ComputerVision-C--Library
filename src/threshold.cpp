#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>
#include "../include/greyscale.h"
#include "../include/filters/median_filter.h"
#include "../include/threshold.h"

using cv::Mat, cv::Rect, cv::Scalar;
using std::cout, std::endl;
using std::vector;

thresh::thresh(Mat image)
{
	img = image;
	greyscale grey(img);
	grayscaled = Mat(grey.toGrayScale()); //Grayscale the image
}
void thresh::optimalThreshold(Mat image)
{
	vector<int> mask1, mask2;
	int mask1_mean, mask2_mean, new_threshold, old_threshold = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{

			if ((i == 0 && j == 0) || (i == 0 && j == image.cols - 1) || (i == image.rows - 1 && j == 0) || (i == image.rows - 1 && j == image.cols - 1))
				mask1.push_back(image.at<uchar>(i, j));
			else
				mask2.push_back(image.at<uchar>(i, j));
		}
	}
	mask1_mean = std::accumulate(mask1.begin(), mask1.end(), 0.0) / mask1.size();
	mask2_mean = std::accumulate(mask2.begin(), mask2.end(), 0.0) / mask2.size();
	new_threshold = (mask1_mean + mask2_mean) / 2;
	while (new_threshold != old_threshold)
	{
		old_threshold = new_threshold;
		for (int i = 0; i < image.rows; i++)
			for (int j = 0; j < image.cols; j++)
			{
				{
					if (image.at<uchar>(i, j) < new_threshold)
						mask1.push_back(image.at<uchar>(i, j));
					else
						mask2.push_back(image.at<uchar>(i, j));
				}
			}
		mask1_mean = std::accumulate(mask1.begin(), mask1.end(), 0.0) / mask1.size();
		mask2_mean = std::accumulate(mask2.begin(), mask2.end(), 0.0) / mask2.size();
		new_threshold = (mask1_mean + mask2_mean) / 2;
	}
	threshold = new_threshold;
}
void thresh::otsuThreshold(Mat image)
{
	int i, j, k;
	int size;
	//histogram array initialized.
	int *histogram = new int[256];

	// Initial values zero
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	int rowCount = image.rows;
	int columnCount = image.cols;
	size = columnCount * rowCount;
	//Histogram calculated.
	for (i = 0; i < image.rows; i++)
	{

		for (j = 0; j < image.cols; j++)
		{
			histogram[(int)image.at<uchar>(i, j)]++;
		}
	}
	//Variables initialized
	double w0 = 0;
	float w1 = 0;
	float m0 = 0;
	float m1 = 0;
	float variance = 0;
	float variancemax = 0;
	//otsu algorithm
	for (j = 0; j < 256; j++)
	{
		w0 = 0;
		//Means for both class calculated..
		//WO first class
		for (k = 0; k < j; k++)
		{
			w0 += (histogram[k]);
		}
		//W1 second class
		w1 = size - w0; //size of image (w1 = 1- w0)
		int sum = 0;
		//Sum of class for calculating variance for first class
		for (k = 0; k < j; k++)
		{
			sum += k * (histogram[k]);
		}
		//Variances for first class
		m0 = sum / w0;
		sum = 0;
		//Sum of class for calculating variance for second class
		for (k = j; k < 256; k++)
		{
			sum += k * histogram[k];
		}
		//Variances for second class
		m1 = sum / w1;
		variance = sqrt(w0 * w1 * (m0 - m1) * (m0 - m1));
		//Maximum between class variances values
		if (variance > variancemax)
		{
			threshold = j;
			variancemax = variance;
		}
	}
}
Mat thresh::spectralThreshold(Mat image)
{
	// 3 modes otsu implementation

	int i, j, k;
	int size;
	//histogram array initialized.
	int *histogram = new int[256];

	// Initial values zero
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	int rowCount = image.rows;
	int columnCount = image.cols;
	size = columnCount * rowCount;
	//Histogram calculated.
	for (i = 0; i < image.rows; i++)
	{

		for (j = 0; j < image.cols; j++)
		{
			histogram[(int)image.at<uchar>(i, j)]++;
		}
	}
	double w0, w1, w2, m0, m1, m2, currVarB, threshold1, threshold2, maxBetweenVar, m0K, m1K, m2K, mt;

	threshold1 = 0;
	threshold2 = 0;

	w0 = 0;
	w1 = 0;

	m0K = 0;
	m1K = 0;

	mt = 0;
	maxBetweenVar = 0;
	for (int k = 0; k <= 255; k++)
	{
		mt += k * (histogram[k] / (double)size);
	}

	for (int t1 = 0; t1 <= 255; t1++)
	{
		w0 += histogram[t1] / (double)size;		//Pi
		m0K += t1 * (histogram[t1] / (double)size); //i * Pi
		m0 = m0K / w0;								//(i * Pi)/Pi

		w1 = 0;
		m1K = 0;

		for (int t2 = t1 + 1; t2 <= 255; t2++)
		{
			w1 += histogram[t2] / (double)size;		//Pi
			m1K += t2 * (histogram[t2] / (double)size); //i * Pi
			m1 = m1K / w1;								//(i * Pi)/Pi

			w2 = 1 - (w0 + w1);
			m2K = mt - (m0K + m1K);

			if (w2 <= 0)
				break;

			m2 = m2K / w2;

			currVarB = w0 * (m0 - mt) * (m0 - mt) + w1 * (m1 - mt) * (m1 - mt) + w2 * (m2 - mt) * (m2 - mt);

			if (maxBetweenVar < currVarB)
			{
				maxBetweenVar = currVarB;
				threshold1 = t1;
				threshold2 = t2;
			}
		}
	}
	thresholdImage = Mat(image.rows, image.cols, CV_8UC1);
	int maxValue = 255;
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			if (image.at<uchar>(i, j) > threshold2)
				thresholdImage.at<uchar>(i, j) = maxValue;
			else if (image.at<uchar>(i, j) > threshold1)
				thresholdImage.at<uchar>(i, j) = (uchar)maxValue / 2;
			else
				thresholdImage.at<uchar>(i, j) = 0;
		}

	return thresholdImage;
}

Mat thresh::globalThreshold(Mat image)
{
	thresholdImage = Mat(image.rows, image.cols, CV_8UC1);

	int maxValue = 255;

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			if (image.at<uchar>(i, j) > threshold)

				thresholdImage.at<uchar>(i, j) = maxValue;
			else
				thresholdImage.at<uchar>(i, j) = 0;
		}

	return thresholdImage;
}
Mat thresh::localThreshold(int kernal)
{
	int mask_size = kernal;
	Mat mask(mask_size, mask_size, CV_8UC1, Scalar(0));
	Mat output(grayscaled.rows, grayscaled.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < grayscaled.rows - mask_size; i += mask_size)
	{
		for (int j = 0; j < grayscaled.cols - mask_size; j += mask_size)
		{
			mask = grayscaled(Rect(j, i, mask_size, mask_size));
			Mat roi = Mat(mask_size, mask_size, CV_8UC1, Scalar(0));

			if (thresholdMode == "Optimal")
			{
				optimalThreshold(mask);
				roi = globalThreshold(mask);
			}
			else if (thresholdMode == "Otsu")
			{
				otsuThreshold(mask);
				roi = globalThreshold(mask);
			}
			else if (thresholdMode == "Spectral")
				roi = spectralThreshold(mask);

			roi.copyTo(output(cv::Rect(j, i, roi.cols, roi.rows)));
		}
	}
	return output;
}