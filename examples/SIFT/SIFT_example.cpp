#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>

#include <opencv2/core/types.hpp>
#include <vector>

int main()
{
  cv::Mat img = cv::imread("feature_building.jpg");
  cv::Mat gray;
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

  auto sift = cv::SIFT::create();

  std::vector<cv::KeyPoint> kp;

  sift->detect(gray, kp);

  cv::drawKeypoints(gray, kp, img);

  cv::imwrite("C_OpenCV_SIFT.jpg", img);

  return 0;
}
