#pragma once
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std; 

#include <vector>
class Polygon {
public:
	vector<Point2d> points;
	vector<Point2d> regularPoints;
	Polygon(); 
	Polygon(vector<Point2d> points);
	void update(const Mat &G);
	void addPoint(Point2d p); // adds a point to the polygon.
	Mat drawPolygon(const Mat &Image); //draws the polygon on a given image.
	void drawNormales(const Mat &Image);
	void drawCurvatures(const Mat &Image);
	vector<Point2d> rechantillonage(); //adds regular points on the polygon.
	vector<Vec2d> computeTangentes();
	vector<Vec2d> computeNormales();
	vector<Vec2d> computeDeriv();
	vector<double> computeCurvature();
	Vec2d getNormale(int i);
	double getCurvature(int i);
	Point2d getPoint(int i);
	void replacePoint(int i, Point2d p);
	int getRegularPointSize() {return regularPoints.size();};
	vector<Point2d> deleteLoop();
	~Polygon(); //destructor

private:
	vector<Vec2d> tangentes; 
	vector<Vec2d> normales; 
	vector<Vec2d> deriveeseconde; 
	vector<double> curvature;
};