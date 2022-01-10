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

void App::btn_open()
{
  if (imgview_file_dialog)
    imgview_file_dialog->show();
}

void App::btn_edge()
{
  string edge_type = entry_edge->get_text();
  if(edge_type == "Canny")
  {
    // TODO
    cout<<"Canny"<<endl;
    return;
  }
  else if(edge_type == "Hough")
  {
    string thresh = Threshold->get_text();
    uint threshold;
    try
    {
      threshold = std::stoi(thresh);
    }
    catch (std::exception &excp)
    {cout<<"Entry error"<<endl;}
    // cout<<"Threshold "<< threshold<<endl;
    imgwin2.change_image(hough_circle(img_mat1, threshold));
    cout<<"Hough"<<endl;
    return;
  }
  else if(edge_type == "Snake")
  {
    snake snake_cnt(img_mat1);
    imgwin2.change_image(snake_cnt.snakeContour());
    vector<Pointxy> points;
    for (int i = 0 ; i<snake_cnt.snakePolygon.data.size();i++)
    {
        // cout << "x: " << snake_cnt.snakePolygon.data[i].x << ", ";
        // cout << "y: " << snake_cnt.snakePolygon.data[i].y << endl;
        Pointxy pnt(snake_cnt.snakePolygon.data[i].x, snake_cnt.snakePolygon.data[i].y);
        points.push_back(pnt);
    }
    contour.set_points(points);
    return;
  }
}

void App::btn_operation()
{
  string edge_type = entry_operation->get_text();
  if(edge_type == "Get perimeter")
  {
    if (!contour.empty())
    {
      contour.sort_points();
      double perimeter = contour.get_perimeter();
      Area_peri->set_text(to_string(perimeter));
      // cout<<"Perimeter is "<<perimeter<<endl;

    }
    return;
  }

  else if(edge_type == "Get area")
  {
    if (!contour.empty())
    {
      contour.sort_points();
      double area = contour.get_area();
      Area_peri->set_text(to_string(area));
      // cout<<"Area is "<<area<<endl;
    }
    return;
  }
  return;
}