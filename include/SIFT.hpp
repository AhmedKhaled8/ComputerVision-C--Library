#ifndef SIFT_HPP
#define SIFT_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

struct xyoctscale
{
  uint x = 0;
  uint y = 0;
  uint o = 0;
  uint s = 0;
};

struct xyoctscaleangle
{
    uint x = 0;
    uint y = 0;
    uint o = 0;
    uint s = 0;
    float angle = 0;
};

struct xyscaleangledescriptor
{
    uint x = 0;
    uint y = 0;
    uint s = 0;
    float angle = 0;
    std::vector<float> feature_desc;
};

#endif //SIFT_HPP
