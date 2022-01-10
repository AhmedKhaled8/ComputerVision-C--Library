#ifndef KMEANS_H
#define KMEANS_H

#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>

#define MIN_CENTROID_DISTANCE 100
#define CV_BGR_VECTOR cv::Vec3b
#define BLUE 0
#define GREEN 1
#define RED 2

struct BGR_point{
  int b;
  int g;
  int r;
  int & operator[](int idx)
  {
    switch (idx)
    {
      case 0:
      return b;
      case 1:
      return g;
      case 2:
      return r;
    }
  }
};


typedef struct{
    std::vector<BGR_point> centroids;
    std::vector<int> clusters;
    cv::Mat segmented_image;
} KMeans_result;


void print_centroids(std::vector<BGR_point> centroids);
double calculate_euclidean_distance(BGR_point p1, BGR_point p2);
std::vector<BGR_point> generate_random_centroids(int number_of_centroids, cv::Mat image);
std::vector<BGR_point> create_image_data(cv::Mat image);
int get_closest_cluster(BGR_point point, std::vector<BGR_point> centroids);
std::vector<int> assign_clusters(std::vector<BGR_point> image_data, std::vector<BGR_point> centroids);
std::vector<BGR_point> update_centroids(std::vector<BGR_point> image_data, std::vector<int> clusters, std::vector<BGR_point> centroids);
bool is_converged(std::vector<BGR_point> old_centroids, std::vector<BGR_point> new_centroids);
void segment_image(cv::Mat image, KMeans_result &kmeans);
KMeans_result apply_kmeans(cv::Mat image, int num_clusters, int max_iterations);

#endif //KMEANS_H
