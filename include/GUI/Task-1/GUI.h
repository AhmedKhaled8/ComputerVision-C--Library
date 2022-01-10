#ifndef GUI_H // include guard
#define GUI_H

#include <gtkmm.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
using std::cout, std::endl, std::string;
using namespace cv;

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();

  void change_image(string filepath);
  void change_image(Glib::RefPtr<Gdk::Pixbuf> image);
  void change_image(Mat image);

protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

  Glib::RefPtr<Gdk::Pixbuf> m_image;
};


class App : public Gtk::Application
{

public:
  App();
  ~App();

  Gtk::Window* window;

protected:
  //Member widgets:
  Glib::RefPtr<Gtk::Builder> builder;

  Gtk::Fixed* fixed;

  Gtk::FileChooserDialog* hybrid_file_dialog;
  Gtk::FileChooserDialog* imgview_file_dialog;

  Gtk::Button* b_open;
  Gtk::Button* b_sobel;
  Gtk::Button* b_roberts;
  Gtk::Button* b_prewitt;
  Gtk::Button* b_canny;
  Gtk::Button* b_draw_hist;
  Gtk::Button* b_equalize;
  Gtk::Button* b_normalize;
  Gtk::Button* b_rhist;
  Gtk::Button* b_ghist;
  Gtk::Button* b_bhist;
  Gtk::Button* b_local_thresh;
  Gtk::Button* b_global_thresh;
  Gtk::Button* b_grayscale;
  Gtk::Button* b_low_pass;
  Gtk::Button* b_high_pass;
  Gtk::Button* b_hybrid;
  Gtk::Button* b_rcum;
  Gtk::Button* b_gcum;
  Gtk::Button* b_bcum;
  Gtk::Button* b_uni_noise;
  Gtk::Button* b_gauss_noise;
  Gtk::Button* b_sp_noise;
  Gtk::Button* b_avg_filter;
  Gtk::Button* b_gauss_filter;
  Gtk::Button* b_med_filter;

  Gtk::EventBox* imbox;
  Gtk::EventBox* imbox2;
  MyArea imgwin1;
  MyArea imgwin2;

  Mat img_mat;

protected:
  // Signal handlers:
  void hybrid_chose_file (int response_id);
  void imgview_chose_file (int response_id);

  void btn_open();
  void btn_sobel();
  void btn_roberts();
  void btn_prewitt();
  void btn_canny();
  void btn_draw_hist();
  void btn_equalize();
  void btn_normalize();
  void btn_rhist();
  void btn_ghist();
  void btn_bhist();
  void btn_local_thresh();
  void btn_global_thresh();
  void btn_grayscale();
  void btn_low_pass();
  void btn_high_pass();
  void btn_hybrid();
  void btn_rcum();
  void btn_gcum();
  void btn_bcum();
  void btn_uni_noise();
  void btn_gauss_noise();
  void btn_sp_noise();
  void btn_avg_filter();
  void btn_gauss_filter();
  void btn_med_filter();

  bool imgbox(GdkEventButton* event);

protected:
  // Member initializer functions
  void get_widgets();
  void connect_signals();
};

#endif