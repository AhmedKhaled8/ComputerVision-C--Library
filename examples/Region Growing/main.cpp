#include "../include/segmentation/region_growing.h"

int main() {
    cv::Mat img = cv::imread("Brain_ct_8.png", 0);
    cv::Mat constructed_image = apply_region_growing(img, 4);
    cv::imshow("Output", constructed_image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
