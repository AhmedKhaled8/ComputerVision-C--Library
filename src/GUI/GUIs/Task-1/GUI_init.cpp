#include "../../include/GUI/GUI.h"

// MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
MyArea::MyArea()
{
  m_image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, 300, 250);
}

MyArea::~MyArea()
{
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (!m_image)
    return false;

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  cr->scale((double)width/m_image->get_width(),(double)height/m_image->get_height());
  Gdk::Cairo::set_source_pixbuf(cr, m_image, 0,0);
  cr->rectangle(0, 0, m_image->get_width(), m_image->get_height());
  cr->fill();

  return true;
}

void MyArea::change_image(string filepath)
{
  try
  {
    m_image = Gdk::Pixbuf::create_from_file(filepath);
  }
  catch(const Gio::ResourceError& ex)
  {
    std::cerr << "ResourceError: " << ex.what() << std::endl;
  }
  catch(const Gdk::PixbufError& ex)
  {
    std::cerr << "PixbufError: " << ex.what() << std::endl;
  }
}

void MyArea::change_image(Glib::RefPtr<Gdk::Pixbuf> image)
{
  m_image = image;
}

void MyArea::change_image(Mat img)
{
  Mat image;
  cv::cvtColor(img, image, COLOR_BGR2RGB);
  m_image = Gdk::Pixbuf::create_from_data((guint8*) image.data, Gdk::COLORSPACE_RGB,
   image.channels() == 4, 8, image.size().width, image.size().height,image.step)
   ->scale_simple(get_allocation().get_width(),get_allocation().get_height() , Gdk::INTERP_BILINEAR );
   queue_draw();
}

void App::get_widgets()
{
  builder = Gtk::Builder::create_from_file("../design/task1_gui.glade");

  builder->get_widget("window",window);
  builder->get_widget("fixed",fixed);
  builder->get_widget("hybrid_file_dialog",hybrid_file_dialog);
  builder->get_widget("imgview_file_dialog",imgview_file_dialog);

  builder->get_widget("btn_open",b_open);
  builder->get_widget("btn_sobel",b_sobel);
  builder->get_widget("btn_roberts",b_roberts);
  builder->get_widget("btn_prewitt",b_prewitt);
  builder->get_widget("btn_canny",b_canny);
  builder->get_widget("btn_draw_hist",b_draw_hist);
  builder->get_widget("btn_equalize",b_equalize);
  builder->get_widget("btn_normalize",b_normalize);
  builder->get_widget("btn_rhist",b_rhist);
  builder->get_widget("btn_ghist",b_ghist);
  builder->get_widget("btn_bhist",b_bhist);
  builder->get_widget("btn_local_thresh",b_local_thresh);
  builder->get_widget("btn_global_thresh",b_global_thresh);
  builder->get_widget("btn_grayscale",b_grayscale);
  builder->get_widget("btn_low_pass",b_low_pass);
  builder->get_widget("btn_high_pass",b_high_pass);
  builder->get_widget("btn_hybrid",b_hybrid);
  builder->get_widget("btn_rcum",b_rcum);
  builder->get_widget("btn_gcum",b_gcum);
  builder->get_widget("btn_bcum",b_bcum);
  builder->get_widget("btn_uni_noise",b_uni_noise);
  builder->get_widget("btn_gauss_noise",b_gauss_noise);
  builder->get_widget("btn_sp_noise",b_sp_noise);
  builder->get_widget("btn_avg_filter",b_avg_filter);
  builder->get_widget("btn_gauss_filter",b_gauss_filter);
  builder->get_widget("btn_med_filter",b_med_filter);


  builder->get_widget("imgbox", imbox);
  builder->get_widget("imgbox2", imbox2);

  imbox->add(imgwin1);
  imgwin1.set_size_request(300,250);
  imgwin1.show();
  imbox2->add(imgwin2);
  imgwin2.set_size_request(300,250);
  imgwin2.show();
}

void App::connect_signals()
{
  hybrid_file_dialog->signal_response().connect(sigc::mem_fun(*this, &App::hybrid_chose_file));
  imgview_file_dialog->signal_response().connect(sigc::mem_fun(*this, &App::imgview_chose_file));

  b_open->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_open));
  b_sobel->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_sobel));
  b_roberts->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_roberts));
  b_prewitt->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_prewitt));
  b_canny->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_canny));
  b_draw_hist->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_draw_hist));
  b_equalize->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_equalize));
  b_normalize->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_normalize));
  b_rhist->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_rhist));
  b_ghist->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_ghist));
  b_bhist->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_bhist));
  b_local_thresh->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_local_thresh));
  b_global_thresh->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_global_thresh));
  b_grayscale->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_grayscale));
  b_low_pass->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_low_pass));
  b_high_pass->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_high_pass));
  b_hybrid->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_hybrid));
  b_rcum->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_rcum));
  b_gcum->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_gcum));
  b_bcum->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_bcum));
  b_uni_noise->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_uni_noise));
  b_gauss_noise->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_gauss_noise));
  b_sp_noise->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_sp_noise));
  b_avg_filter->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_avg_filter));
  b_gauss_filter->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_gauss_filter));
  b_med_filter->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_med_filter));

  imbox->signal_button_press_event().connect(sigc::mem_fun(*this, &App::imgbox));
}

App::App()
{
  get_widgets();
  connect_signals();
}

App::~App()
{
}