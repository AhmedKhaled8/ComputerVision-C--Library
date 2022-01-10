#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include "../include/segmentation/region_growing.h"



void on_mouse_click(int event, int x, int y, int flags, void* params){
    if(event == cv::EVENT_LBUTTONDOWN){
        select_seeds &seeds_img_struct = *((select_seeds*) (params));

        point2rc point;
        point.row = y;
        point.col = x;
        seeds_img_struct.seeds.push_back(point);

        cv::circle(seeds_img_struct.img, cv::Point(x, y), 1, cv::Scalar(175), 8);
        cv::imshow("Please select your seed points", seeds_img_struct.img);
    }
}

void get_seeds(select_seeds &seeds_img_struct){
    std::string window_name = "Please select your seed points";
    cv::namedWindow(window_name);
    cv::setMouseCallback(window_name, on_mouse_click, &seeds_img_struct);
    cv::imshow(window_name, seeds_img_struct.img);
    cv::waitKey(0);
    cv::destroyWindow(window_name);
}




std::vector<point2rc> create_orientations(int num_orientations){
    if(num_orientations == 4){
        std::vector<point2rc> orientations;
        point2rc orientation;

        orientation = {1, 0};
        orientations.push_back(orientation);

        orientation = {0, 1};
        orientations.push_back(orientation);

        orientation = {-1, 0};
        orientations.push_back(orientation);

        orientation = {0, -1};
        orientations.push_back(orientation);

        return orientations;
    }
    else {
        std::vector<point2rc> orientations;
        point2rc orientation;

        orientation = {1, 0};
        orientations.push_back(orientation);

        orientation = {1, 1};
        orientations.push_back(orientation);

        orientation = {0, 1};
        orientations.push_back(orientation);

        orientation = {-1, 1};
        orientations.push_back(orientation);

        orientation = {-1, 0};
        orientations.push_back(orientation);

        orientation = {-1, -1};
        orientations.push_back(orientation);

        orientation = {0, 1};
        orientations.push_back(orientation);

        orientation = {1, -1};
        orientations.push_back(orientation);

        return orientations;
    }
}

point2rc get_neighbour_pixel(point2rc pixel, point2rc orientation, RegionGrowing const& region){
    point2rc neighbour;
    neighbour.row = pixel.row + orientation.row;
    neighbour.col = pixel.col + orientation.col;
    bool row_in_image = (neighbour.row >= 0) && (neighbour.row < region.original_img.rows);
    bool col_in_image = (neighbour.col >= 0) && (neighbour.col < region.original_img.cols);
    if(row_in_image && col_in_image){
        return neighbour;
    }
    else{
        neighbour.row = OUTSIDE_LIMIT;
        neighbour.col = OUTSIDE_LIMIT;
        return neighbour;
    }
}

bool is_neighbour_outside_image(point2rc neighbour){
    return (neighbour.row == OUTSIDE_LIMIT || neighbour.col == OUTSIDE_LIMIT);
}

std::vector<point2rc> explore_neighbours(std::vector<point2rc> original_contour, point2rc curr_pixel, RegionGrowing region){
    point2rc neighbour;
    std::vector<point2rc> new_contour = original_contour;
    for(auto &orientation: region.orientations){
        neighbour = get_neighbour_pixel(curr_pixel, orientation, region);
        if(is_neighbour_outside_image(neighbour)){
            // if the neighbour has indexes that aren't compatible with the image shape just ignore it
            continue;
        }
        if(region.segmented_img.at<uchar>(neighbour.row, neighbour.col) == 0){
            // if the neighbour wasn't explored before by any previous pixels (their value at the segmented array is 0
            // add them to the contour and mark they were selected by setting its value in the segmented array to 150 (not 0 any more)
            new_contour.push_back(neighbour);
            region.segmented_img.at<uchar>(neighbour.row, neighbour.col) = 150;
        }
    }
    return new_contour;
}

int get_least_index(std::vector<double> distances){
    int min_index = 0;
    double min_distance = distances[0];
    for(int i = 1; i < distances.size(); ++i){
        if(distances[i] < min_distance){
            min_index = i;
            min_distance = distances[i];
        }
    }
    return min_index;
}

int get_nearest_neighbour(const std::vector<point2rc>& contour, double seg_mean_value, RegionGrowing region, double &dist){
    double orig_value;
    double temp_distance;
    int min_index;
    std::vector<double> distances;
    for(auto &pixel : contour){
        orig_value = region.original_img.at<uchar>(pixel.row, pixel.col);
        temp_distance = std::abs(orig_value - seg_mean_value);
        distances.push_back(temp_distance);
    }
    if(distances.empty()){
        dist = 1000;
        return -1;
    }
    else{
        min_index = get_least_index(distances);
        dist = distances[min_index];
        return min_index;
    }
}

void segment_region(RegionGrowing &region, const std::vector<point2rc>& seeds){
    point2rc current_pixel;
    int seg_size;
    double seg_mean_value;
    double dist;
    int nearest_neighbour_index;
    std::vector<point2rc> contour;
    for(auto &seed: seeds){
        current_pixel.row = seed.row;
        current_pixel.col = seed.col;
        if(region.segmented_img.at<uchar>(current_pixel.row, current_pixel.col) == 255){
            continue;
        }
        contour.clear();
        seg_size = 1;
        seg_mean_value = region.original_img.at<uchar>(current_pixel.row, current_pixel.col);
        dist = 0;
        while(dist <= region.threshold){
            region.segmented_img.at<uchar>(current_pixel.row, current_pixel.col) = 255;
            contour = explore_neighbours(contour, current_pixel, region);
            nearest_neighbour_index = get_nearest_neighbour(contour, seg_mean_value, region, dist);
            if(nearest_neighbour_index == -1){
                break;
            }
            current_pixel.row = contour[nearest_neighbour_index].row;
            current_pixel.col = contour[nearest_neighbour_index].col;
            seg_mean_value = ((seg_mean_value * seg_size) + region.original_img.at<uchar>(current_pixel.row, current_pixel.col)) / (seg_size + 1);
            seg_size += 1;
            contour.erase(contour.begin() + nearest_neighbour_index);
        }
    }
}

cv::Mat construct_region_segmented_image(RegionGrowing region){
    int rows = region.original_img.rows;
    int cols = region.original_img.cols;
    cv::Mat constructed_image = region.original_img.clone();
    uchar original_val;
    uchar segment_val;
    for(int r = 0; r < rows; ++r){
        for(int c = 0; c < cols; ++c){
            original_val = region.original_img.at<uchar>(r, c);
            segment_val = region.segmented_img.at<uchar>(r, c);
            constructed_image.at<uchar>(r, c) = std::max(original_val, segment_val);
        }
    }
    return constructed_image;
}

cv::Mat apply_region_growing(const cv::Mat& original_image, int num_orientations){
    std::vector<point2rc> seeds;
    cv::Mat seeds_img = original_image.clone();
    cv::cvtColor(seeds_img, seeds_img, cv::COLOR_GRAY2BGR);
    select_seeds seeds_img_struct = {seeds_img, seeds};
    get_seeds(seeds_img_struct);
    for(auto &point: seeds_img_struct.seeds){
        std::cout << point.row << " " << point.col << std::endl;
    }
    cv::Mat smoothed_image = original_image.clone();
    cv::GaussianBlur(original_image, smoothed_image, cv::Size(3, 3), 1);
    RegionGrowing region(smoothed_image, num_orientations);
    segment_region(region, seeds_img_struct.seeds);
    cv::Mat constructed_image = construct_region_segmented_image(region);
    return constructed_image;
}
