#ifndef Meanshift_H
#define Meanshift_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class Point5D
{
public:
	float x;	/* Spatial Value (x) */
	float y;	/* Spatial Value (y) */
	float l;	/* L Channel */
	float a;	/* A Channel */
	float b;	/* B Channel */
public:
	Point5D();													/* Constructor */
	void PointLab();											/* LAB Color */
	void PointRGB();											/* RGB Color */
	void MSPoint5DAccum(Point5D);								/* Accumulate Points */
	void MSPoint5DCopy(Point5D);								// Copy a point
	float MSPoint5DColorDistance(Point5D);						// Compute color space distance between two points
	float MSPoint5DSpatialDistance(Point5D);					// Compute spatial space distance between two points
	void MSPoint5DScale(float);									// Scale point
	void MSPOint5DSet(float, float, float, float, float);		// Set point value
};

class MeanShift
{
public:
	float hs;					/* Spatial Radius */
	float hr;					/* Color Radius */
	vector<Mat> IMGChannels;	/* For Separation of Image Channels */
public:
	MeanShift(float, float);	/* Constructor */
	void MSFiltering(Mat&);		/* Mean Shift Filtering Function */
};

#endif