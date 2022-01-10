#include "GUI/GUI.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "greyscale.h"
#include "threshold.h"
#include "edges/roberts.h"
#include "edges/prewitt.h"
#include "edges/sobel.h"
#include "edges/canny.h"
#include "histogram.h"
#include "normalization.h"
#include "equalization.h"
#include "filters/average_filter.h"
#include "filters/gaussian_filter.h"
#include "filters/median_filter.h"
#include "noise/add_noise.h"
#include "FFT_Filter.h"
#include "kernels_filter.h"

using namespace cv;

void App::hybrid_chose_file(int response_id)
{
  hybrid_file_dialog->hide();
  std::string filename;
  //Handle the response:
  switch (response_id)
  {
  // -5 is equiivalent to OK
  case -5:
  {
    //Notice that this is a std::string, not a Glib::ustring.
    filename = hybrid_file_dialog->get_file()->get_path();
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
  Mat matrix2 = imread(filename);
  imgwin2.change_image(get_hybrid(img_mat, matrix2));
}

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
  img_mat = imread(filename);
  imgwin1.change_image(img_mat);
}

void App::btn_open()
{
  if (imgview_file_dialog)
    imgview_file_dialog->show();
}

void App::btn_sobel()
{
  sobel sbl(img_mat);
  imgwin2.change_image(sbl.sFiltered);
}
void App::btn_roberts()
{
  roberts rbrts(img_mat);
  imgwin2.change_image(rbrts.rFiltered);
}
void App::btn_prewitt()
{
  prewitt prwt(img_mat);
  imgwin2.change_image(prwt.pFiltered);
}
void App::btn_canny()
{
  canny cny(img_mat);
  imgwin2.change_image(cny.thres);
}
void App::btn_draw_hist()
{
  greyscale grey(img_mat);
  histogram histo(grey.grayscaled);
  histo.computeHistogram("greyscale");
  imgwin2.change_image(histo.histImage);
}
void App::btn_equalize()
{
  greyscale grey(img_mat);
  equalization equal(grey.grayscaled);
  imgwin2.change_image(equal.equalize());
}
void App::btn_normalize()
{
  greyscale grey(img_mat);
  normalization norm(grey.grayscaled);
  imgwin2.change_image(norm.normalize());
}
void App::btn_rhist()
{
  histogram histo(img_mat);
  histo.computeHistogram("red");
  imgwin2.change_image(histo.histImage);
}
void App::btn_ghist()
{
  histogram histo(img_mat);
  histo.computeHistogram("green");
  imgwin2.change_image(histo.histImage);
}
void App::btn_bhist()
{
  histogram histo(img_mat);
  histo.computeHistogram("blue");
  imgwin2.change_image(histo.histImage);
}
void App::btn_local_thresh()
{
  thresh thres(img_mat);
  thres.localThreshold();
  imgwin2.change_image(thres.thresholdImage);
}
void App::btn_global_thresh()
{
  thresh thres(img_mat);
  thres.globalThreshold();
  imgwin2.change_image(thres.thresholdImage);
}
void App::btn_grayscale()
{
  greyscale grey(img_mat);
  imgwin2.change_image(grey.grayscaled);
}
void App::btn_low_pass()
{
  imgwin2.change_image(fft_filter(img_mat, 'l'));
}
void App::btn_high_pass()
{
  imgwin2.change_image(fft_filter(img_mat, 'h'));
}
void App::btn_hybrid()
{
  if (hybrid_file_dialog)
    hybrid_file_dialog->show();
}
void App::btn_rcum()
{
  histogram histo(img_mat);
  histo.computeCumulative("red");
  imgwin2.change_image(histo.cumImage);
}
void App::btn_gcum()
{
  histogram histo(img_mat);
  histo.computeCumulative("green");
  imgwin2.change_image(histo.cumImage);
}
void App::btn_bcum()
{
  histogram histo(img_mat);
  histo.computeCumulative("blue");
  imgwin2.change_image(histo.cumImage);
}

bool App::imgbox(GdkEventButton *event)
{
  return true;
}

void App::btn_uni_noise()
{
  imgwin2.change_image(uniform_noise(img_mat));
}
void App::btn_gauss_noise()
{
  imgwin2.change_image(gauss_noise(img_mat));
}
void App::btn_sp_noise()
{
  imgwin2.change_image(salt_pepper_noise(img_mat));
}
void App::btn_avg_filter()
{
  imgwin2.change_image(avg_filter(img_mat));
}
void App::btn_gauss_filter()
{
  gaussian gauss(img_mat);
  imgwin2.change_image(gauss.gFiltered);
}
void App::btn_med_filter()
{
  imgwin2.change_image(med_filter(img_mat));
}