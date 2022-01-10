#include "GUI/GUI.h"
#include <iostream>
#include <string>
#include <vector>
#include "greyscale.h"
#include "threshold.h"
#include "edges/roberts.h"
#include "edges/prewitt.h"
#include "edges/sobel.h"
#include "edges/canny.h"
#include "snake.h"
#include "Hough.h"
#include "matching/SSD.h"
#include "matching/NCC.h"
#include "SIFT.h"
#include "harris.h"

using std::cout, std::endl, std::string;
using cv::Mat, cv::imread;

void App::imgview_chose_file(int response_id)
{
  imgview_file_dialog->hide();
  std::string filename;
  //Handle the response:
  switch (response_id)
  {
  // -5 is equiivalent to OK
  case -5:
  {
    //Notice that this is a std::string, not a Glib::ustring.
    filename = imgview_file_dialog->get_file()->get_path();
    // std::cout << "File selected: " <<  filename << std::endl;
    break;
  }
  // -6 is equiivalent to cancel
  case -6:
  {
    std::cout << "Cancel clicked." << std::endl;
    return;
  }
  default:
  {
    return;
  }
  }
  img_mat1 = imread(filename);
  imgwin1.change_image(img_mat1);
}

void App::imgview_chose_file2(int response_id)
{
  imgview_file_dialog2->hide();
  std::string filename;
  //Handle the response:
  switch (response_id)
  {
  // -5 is equiivalent to OK
  case -5:
  {
    //Notice that this is a std::string, not a Glib::ustring.
    filename = imgview_file_dialog2->get_file()->get_path();
    // std::cout << "File selected: " <<  filename << std::endl;
    break;
  }
  // -6 is equiivalent to cancel
  case -6:
  {
    std::cout << "Cancel clicked." << std::endl;
    return;
  }
  default:
  {
    return;
  }
  }
  img_mat2 = imread(filename);
  imgwin2.change_image(img_mat2);
}
void App::btn_open()
{
  if (imgview_file_dialog)
    imgview_file_dialog->show();
}
void App::btn_open2()
{
  if (imgview_file_dialog2)
    imgview_file_dialog2->show();
}

void App::btn_feature()
{
  string feature_type = entry_feature->get_text();
  if(feature_type == "Harris")
  {
    string thresh = Threshold->get_text();
    uint threshold;
    try
    {
      threshold = std::stoi(thresh);
      cout << "Harris" << endl;
    }
    catch (std::exception &excp)
    {cout<<"Entry error"<<endl;}    
    return;
  }
  else if(feature_type == "SIFT")
  {
    long long begin = clock();
    sift siftfeat(img_mat1);
    imgwin2.change_image(siftfeat.sift_Image);
    long long end = clock();
    Glib::ustring time = to_string((end - begin) * 1000.0 / CLOCKS_PER_SEC) + " ms"; 
    Threshold->set_text(time);
  }
}

void App::btn_match()
{
  string match_type = entry_match->get_text();
  if(match_type == "Sum of squared difference")
  {
    sift siftfeat1(img_mat1);
    sift siftfeat2(img_mat2);
    long long begin = clock();
    SSD ssd(siftfeat1.sift_Image, siftfeat2.sift_Image, siftfeat1.keypoints, siftfeat2.keypoints);
    ssd.testSSD();
    long long end = clock();
    Glib::ustring time = to_string((end - begin) * 1000.0 / CLOCKS_PER_SEC) + " ms"; 
    Threshold->set_text(time);
    return;
  }

  else if(match_type == "Normalized cross correlations")
  {
    sift siftfeat1(img_mat1);
    sift siftfeat2(img_mat2);
    long long begin = clock();
    NCC ncc(siftfeat1.sift_Image, siftfeat2.sift_Image, siftfeat1.keypoints, siftfeat2.keypoints);
    ncc.testNCC();
    long long end = clock();
    Glib::ustring time = to_string((end - begin) * 1000.0 / CLOCKS_PER_SEC) + " ms"; 
    Threshold->set_text(time);
    return;
  }
  return;
}