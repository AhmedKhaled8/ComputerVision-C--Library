#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "contour.h"
using namespace std;

int main() {
    cout << "New Code Cool" << endl;
    Point p1(-5, 1);
    Point p2(0, 0);
    Point p3(-5, 0);
    Point p4(0, 3);
    Point p5(-2, 3);

    vector<Point> points {p1, p2, p3, p4, p5};

    sort_points(points);
    for(int i = 0; i < points.size(); i++)
    {
        cout << "(" << points[i].x << "," << points[i].y << ")" << endl;
    }
    cout << get_area(points) << endl;
    cout << get_perimeter(points) << endl;
    return 0;
}
