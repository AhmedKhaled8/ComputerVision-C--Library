#ifndef REGION_GROWING_H
#define REGION_GROWING_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>

#define OUTSIDE_LIMIT -1

typedef struct{
    int row;
    int col;
} point2rc;

typedef struct{
    cv::Mat img;
    std::vector<point2rc> seeds;
} select_seeds;

std::vector<point2rc> create_orientations(int num_orientations);

class RegionGrowing{
public:
    cv::Mat original_img;
    cv::Mat segmented_img;
    int threshold;
    int num_orientations;
    std::vector<point2rc> orientations;
    RegionGrowing(cv::Mat img, int num_orient, int thresh=20){
        original_img = img;
        segmented_img = cv::Mat::zeros(cv::Size(img.rows, img.cols), CV_8UC1);
        threshold = thresh;
        num_orientations = num_orient;
        orientations = create_orientations(num_orient);
    }
};

void on_mouse_click(int event, int x, int y, int flags, void* params);
void get_seeds(select_seeds &seeds_img_struct);
point2rc get_neighbour_pixel(point2rc pixel, point2rc orientation, const RegionGrowing& region);
bool is_neighbour_outside_image(point2rc neighbour);
std::vector<point2rc> explore_neighbours(std::vector<point2rc> original_contour, point2rc curr_pixel, RegionGrowing region);
int get_least_index(std::vector<double> distances);
int get_nearest_neighbour(const std::vector<point2rc>& contour, double seg_mean_value, RegionGrowing region, double &dist);
void segment_region(RegionGrowing &region, const std::vector<point2rc>& seeds);
cv::Mat construct_region_segmented_image(RegionGrowing region);
cv::Mat apply_region_growing(const cv::Mat& original_image, int num_orientations);

#endif //REGION_GROWING_H
