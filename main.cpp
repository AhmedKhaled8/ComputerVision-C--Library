#include "GUI/GUI.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
  App* app= new App;
  app->create("org.BAA.Task1");
  return app->run(*app->window);
}
