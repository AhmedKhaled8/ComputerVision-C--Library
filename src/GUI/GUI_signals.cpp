#include "GUI/GUI.h"
#include <iostream>
#include <string>
#include <vector>
#include "threshold.h"
#include "matching/SSD.h"
#include "matching/NCC.h"
#include "SIFT.h"
#include "harris.h"

using cv::Mat, cv::imread;
using std::cout, std::endl, std::string;

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

void App::btn_open()
{
  if (imgview_file_dialog)
    imgview_file_dialog->show();
}

void App::btn_segment()
{
  string entry = entry_segment->get_text();
  if (entry == "Kmeans" || entry == "Mean shift" || entry == "Region growing" || entry == "Agglomerative")
  {
    string nsegs_text = num_segs->get_text();
    uint nsegs;
    try
    {
      nsegs = std::stoi(nsegs_text);
      cout << entry << endl;
    }
    catch (std::exception &excp)
    {
      cout << "Entry error" << endl;
      return;
    }

    if (entry == "Kmeans")
    {
      // TODO
    }
    else if (entry == "Mean shift")
    {
      // TODO
    }
    else if (entry == "Region growing")
    {
      // TODO
    }
    else if (entry == "Agglomerative")
    {
      // TODO
    }
  }
}

void App::btn_threshold()
{
  string entry = entry_threshold->get_text();
  string global = entry_global->get_text();
  string kernel_text = entry_kernel->get_text();
  uint ksize;
  if (global == "Global")
  {
    if (entry == "Optimal")
    {

      thresh threshold(img_mat1);
      threshold.optimalThreshold(threshold.grayscaled);
      imgwin2.change_image(threshold.globalThreshold(threshold.grayscaled));
    }

    else if (entry == "Otsu")
    {

      thresh threshold(img_mat1);
      threshold.otsuThreshold(threshold.grayscaled);
      imgwin2.change_image(threshold.globalThreshold(threshold.grayscaled));
    }
    else if (entry == "Spectral")
    {
      thresh threshold(img_mat1);
      imgwin2.change_image(threshold.spectralThreshold(threshold.grayscaled));
    }
  }
  else if (global == "Local")
  {
    try
    {
      ksize = std::stoi(kernel_text);

      if (entry == "Optimal")
      {

        thresh threshold(img_mat1);
        threshold.thresholdMode = "Optimal";
        imgwin2.change_image(threshold.localThreshold(ksize));
      }
      else if (entry == "Otsu")
      {

        thresh threshold(img_mat1);
        threshold.thresholdMode = "Otsu";
        imgwin2.change_image(threshold.localThreshold(ksize));
      }
      else if (entry == "Spectral")
      {
        thresh threshold(img_mat1);
        threshold.thresholdMode = "Spectral";
        imgwin2.change_image(threshold.localThreshold(ksize));
      }
    }
    catch (std::exception &excp)
    {
      cout << "Kernel size entry error" << endl;
      return;
    }
  }

  return;
}
