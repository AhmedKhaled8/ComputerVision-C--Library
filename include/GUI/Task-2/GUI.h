#ifndef GUI_H // include guard
#define GUI_H

#include <gtkmm.h>
#include <iostream>
#include "../contour.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
using std::cout, std::endl, std::string;
using cv::Mat;

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

  Gtk::Viewport* viewport;
  Gtk::Grid* grid;

  Gtk::FileChooserDialog* imgview_file_dialog;

  Gtk::Entry* entry_edge;
  Gtk::Entry* entry_operation;
  Gtk::Entry* Threshold;
  // Gtk::ComboBox* combo_edge;
  // Gtk::ComboBox* combo_operation;

  Gtk::Button* b_open;
  Gtk::Button* b_edge;
  Gtk::Button* b_operation;

  Gtk::Label* Area_peri;

  Gtk::EventBox* imbox1;
  Gtk::EventBox* imbox2;
  MyArea imgwin1;
  MyArea imgwin2;

  Mat img_mat1;
  Mat img_mat2;

  Contour contour;

protected:
  // Signal handlers:
  void imgview_chose_file (int response_id);

  void btn_open();
  void btn_edge();
  void btn_operation();

  bool imgbox(GdkEventButton* event);

protected:
  // Member initializer functions
  void get_widgets();
  void connect_signals();
};

#endif