#include "../include/contour.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

double arctan(Pointxy p1, Pointxy p2)
{
    /**
     * Get the tan-1 angle between 2 data in degrees
     *
     * @param p1 <Pointxy> the first point
     * @param p2 <Pointxy> the second point
     * @return angle the angle in degrees
     */
    double angle = atan2(p1.y - p2.y, p1.x - p2.x) * 180 / PI;
    return angle;
}

bool compare_angle(Pointxy p1, Pointxy p2)
{
    /**
     * Comparison function to be used int std::sort to sort the points
     *
     * @param p1 <Pointxy> the first point
     * @param p2 <Pointxy> the second point
     * @return true if the angle between p1 and the centroid of the polygon centroid is less than the angle between
     *         p2 and the centroid, false if otherwise
     */
    return (p1.angle < p2.angle);
}


bool Contour::empty()
{
  return data.empty();
}

Pointxy Contour::sort_points()
{
    /**
     * sort a vector of points of class Point ccw
     * Needed for the shoelace and perimeter algorithms
     * @param points vector<Pointxy> a vector of points to be sorted
     * @return centroid of class Point: the centroid of the points in the vector
     */
    int n = data.size();
    double x_centroid = 0, y_centroid = 0;
    for (int i = 0; i < n; i++) {
        x_centroid += data[i].x;
        y_centroid += data[i].y;
    }
    Pointxy centroid(x_centroid / n, y_centroid / n);
    for (int i = 0; i < n; i++)
    {
        data[i].angle = arctan(data[i], centroid);
    }
    sort(data.begin(), data.end(), compare_angle);
    return centroid;
}

double Contour::get_area()
{
    /**
     * get the area of the contour using the shoelace algorithm
     *
     * @param points vector<Pointxy> a vector of ordered points to be sorted
     *
     * @return area double: the area inside the contour
     *
     */
    if (area != 0){return area;}

    double _area = 0.0;
    int n = data.size();
    int j = n - 1;
    for(int i=0; i<n; i++)
    {
        _area += (data[j].x + data[i].x) * (data[j].y - data[i].y);
        j = i;
    }
    area = abs(_area / 2.0);
    return area;
}


double Contour::get_perimeter()
{
    /**
     * get the perimeter of the contour
     *
     * @param points vector<Pointxy> a vector of ordered points to be sorted
     *
     * @return perimeter double: the perimeter of the contour
     *
     */
    if (perimeter != 0){return perimeter;}

    double _perimeter = 0.0;
    int n = data.size();
    int j = n-1;
    double x_diff, y_diff;
    for(int i=0; i<n; i++)
    {
        x_diff = pow(data[i].x - data[j].x, 2);
        y_diff = pow(data[i].y - data[j].y, 2);
        _perimeter += sqrt(x_diff + y_diff);
        j = i;
    }
    perimeter = _perimeter;
    return _perimeter;
}

void Contour::add_point(Pointxy point)
{
  data.push_back(point);
  area = 0;
  perimeter = 0;
}

void Contour::set_points(vector<Pointxy> points)
{
  data = points;
  area = 0;
  perimeter = 0;
}
