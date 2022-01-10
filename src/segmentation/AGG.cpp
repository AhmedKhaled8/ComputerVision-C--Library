#include "../../include/segmentation/AGG.h"
#include <math.h>

AGG::AGG(Mat original_image)
{
	OriginalImg = original_image;
}

void print_centroids(vector<Vec3b> centroids) {
    for (auto& centroid : centroids) {
        cout << "(" << centroid[0] << "," << centroid[1] << "," << centroid[2] << "), " << endl;
    }
}

double calculate_euclidean_distance(Vec3b p1, Vec3b p2) {
    /* When Changing this remember to change #define MIN_CENTROID_DISTANCE 100 if needed */
    double b_distance = (p1[0] - p2[0]) * (p1[0] - p2[0]);
    double g_distance = (p1[1] - p2[1]) * (p1[1] - p2[1]);
    double r_distance = (p1[2] - p2[2]) * (p1[2] - p2[2]);
    double distance = b_distance + g_distance + r_distance;
    return distance;
}
