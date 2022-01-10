#ifndef CONTOUR_H
#define CONTOUR_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define PI 3.14159265
using namespace std;

class Pointxy {
    public:
        double x;
        double y;
        double angle;
        Pointxy(double x_coor, double y_coor) {
            x = x_coor;
            y = y_coor;
            angle = 0.0;
        }
};

struct Contour
{
public:
  bool empty();
  Pointxy sort_points();
  double get_area();
  double get_perimeter();

  void add_point(Pointxy);
  void set_points(vector<Pointxy> points);
  vector<Pointxy> data;


private:
  double area = 0;
  double perimeter = 0;
};

double arctan(Pointxy p1, Pointxy p2);
bool compare_angle(Pointxy p1, Pointxy p2);

#endif //CONTOUR_H
