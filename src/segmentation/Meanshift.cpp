#include "../../include/segmentation/Meanshift.h"
#include <math.h>

using namespace cv;
using namespace std;

#define MS_MAX_NUM_CONVERGENCE_STEPS	5										// up to 10 steps are for convergence
#define MS_MEAN_SHIFT_TOL_COLOR			0.3										// minimum mean color shift change
#define MS_MEAN_SHIFT_TOL_SPATIAL		0.3										// minimum mean spatial shift change

Point5D::Point5D()
{
	/* Initialize (x, y) of image by the end of the image */
	x = -1;
	y = -1;
}

void Point5D::PointLab()
{
	/* LAB Color Space Equation */
	l = l * 100 / 255;
	a = a - 128;
	b = b - 128;
}

void Point5D::PointRGB()
{
	/* RGB Color Space Equation */
	l = l * 255 / 100;
	a = a + 128;
	b = b + 128;
}

void Point5D::MSPoint5DAccum(Point5D Pt) {
	x += Pt.x;
	y += Pt.y;
	l += Pt.l;
	a += Pt.a;
	b += Pt.b;
}

void Point5D::MSPoint5DCopy(Point5D Pt)
{
	x = Pt.x;
	y = Pt.y;
	l = Pt.l;
	a = Pt.a;
	b = Pt.b;
}

float Point5D::MSPoint5DColorDistance(Point5D Pt)
{
	return sqrt((l - Pt.l) * (l - Pt.l) + (a - Pt.a) * (a - Pt.a) + (b - Pt.b) * (b - Pt.b));
}

float Point5D::MSPoint5DSpatialDistance(Point5D Pt)
{
	return sqrt((x - Pt.x) * (x - Pt.x) + (y - Pt.y) * (y - Pt.y));
}

void Point5D::MSPoint5DScale(float scale)
{
	x *= scale;
	y *= scale;
	l *= scale;
	a *= scale;
	b *= scale;
}

void Point5D::MSPOint5DSet(float px, float py, float pl, float pa, float pb)
{
	x = px;
	y = py;
	l = pl;
	a = pa;
	b = pb;
}

MeanShift::MeanShift(float s, float r) {
	hs = s;
	hr = r;
}

void MeanShift::MSFiltering(Mat& Img) {
	int ROWS = Img.rows;			// Get row number
	int COLS = Img.cols;			// Get column number
	split(Img, IMGChannels);		// Split Lab color

	Point5D PtCur;					// Current point
	Point5D PtPrev;					// Previous point
	Point5D PtSum;					// Sum vector of the shift vector
	Point5D Pt;
	int Left;						// Left boundary
	int Right;						// Right boundary
	int Top;						// Top boundary
	int Bottom;						// Bottom boundary
	int NumPts;						// number of points in a hypersphere
	int step;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			Left = (j - hs) > 0 ? (j - hs) : 0;						// Get Left boundary of the filter
			Right = (j + hs) < COLS ? (j + hs) : COLS;				// Get Right boundary of the filter
			Top = (i - hs) > 0 ? (i - hs) : 0;						// Get Top boundary of the filter
			Bottom = (i + hs) < ROWS ? (i + hs) : ROWS;				// Get Bottom boundary of the filter
			// Set current point and scale it to Lab color range
			PtCur.MSPOint5DSet(i, j, (float)IMGChannels[0].at<uchar>(i, j), (float)IMGChannels[1].at<uchar>(i, j), (float)IMGChannels[2].at<uchar>(i, j));
			PtCur.PointLab();
			step = 0;				// count the times
			do {
				PtPrev.MSPoint5DCopy(PtCur);						// Set the original point and previous one
				PtSum.MSPOint5DSet(0, 0, 0, 0, 0);					// Initial Sum vector
				NumPts = 0;											// Count number of points that satisfy the bandwidths
				for (int hx = Top; hx < Bottom; hx++) {
					for (int hy = Left; hy < Right; hy++) {
						// Set point in the spatial bandwidth
						Pt.MSPOint5DSet(hx, hy, (float)IMGChannels[0].at<uchar>(hx, hy), (float)IMGChannels[1].at<uchar>(hx, hy), (float)IMGChannels[2].at<uchar>(hx, hy));
						Pt.PointLab();

						// Check it satisfied color bandwidth or not
						if (Pt.MSPoint5DColorDistance(PtCur) < hr) {
							PtSum.MSPoint5DAccum(Pt);				// Accumulate the point to Sum vector
							NumPts++;								// Count
						}
					}
				}
				PtSum.MSPoint5DScale(1.0 / NumPts);					// Scale Sum vector to average vector
				PtCur.MSPoint5DCopy(PtSum);							// Get new origin point
				step++;												// One time end
			// filter iteration to end
			} while ((PtCur.MSPoint5DColorDistance(PtPrev) > MS_MEAN_SHIFT_TOL_COLOR) && (PtCur.MSPoint5DSpatialDistance(PtPrev) > MS_MEAN_SHIFT_TOL_SPATIAL) && (step < MS_MAX_NUM_CONVERGENCE_STEPS));

			// Scale the color
			PtCur.PointRGB();
			// Copy the result to image
			Img.at<Vec3b>(i, j) = Vec3b(PtCur.l, PtCur.a, PtCur.b);
		}
	}
}