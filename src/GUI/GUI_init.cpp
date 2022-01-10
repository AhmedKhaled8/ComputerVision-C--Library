#include "../../include/GUI/GUI.h"

// MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
MyArea::MyArea()
{
  m_image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, 300 , 300);
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
  // cout<<"Width: "<<width<<endl;
  // cout<<"Height: "<<height<<endl;
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
  cv::cvtColor(img, image, cv::COLOR_BGR2RGB);
  m_image = Gdk::Pixbuf::create_from_data((guint8*) image.data, Gdk::COLORSPACE_RGB,
   image.channels() == 4, 8, image.size().width, image.size().height,image.step)
   ->scale_simple(get_allocation().get_width(),get_allocation().get_height() , Gdk::INTERP_BILINEAR );
   queue_draw();
}

App::App()
{
  get_widgets();
  connect_signals();
}

App::~App()
{
}

void App::get_widgets()
{
  builder = Gtk::Builder::create_from_file("../res/task4_gui.glade");

  builder->get_widget("window",window);
  builder->get_widget("viewport",viewport);
  builder->get_widget("grid",grid);
  builder->get_widget("imgview_file_dialog",imgview_file_dialog);

  builder->get_widget("entry_segment",entry_segment);
  builder->get_widget("entry_threshold",entry_threshold);
  builder->get_widget("entry_global",entry_global);
  builder->get_widget("num_segs", num_segs);
  builder->get_widget("entry_kernel", entry_kernel);

  builder->get_widget("btn_open",b_open);
  builder->get_widget("btn_segment",b_segment);
  builder->get_widget("btn_threshold",b_threshold);

  builder->get_widget("imgbox1", imbox1);
  builder->get_widget("imgbox2", imbox2);

  imbox1->add(imgwin1);
  imgwin1.set_size_request(300,250);
  imgwin1.show();
  imbox2->add(imgwin2);
  imgwin2.set_size_request(300,250);
  imgwin2.show();
}

void App::connect_signals()
{
  imgview_file_dialog->signal_response().connect(sigc::mem_fun(*this, &App::imgview_chose_file));

  b_open->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_open));
  b_segment->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_segment));
  b_threshold->signal_clicked().connect(sigc::mem_fun(*this, &App::btn_threshold));
}
