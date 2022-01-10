int main(int argc, char* argv[])
{
  Mat img = cv::imread("../imgs/Simple_circle_r45.png");
  Mat output;
  output = hough_circle(img);
  cv::imshow("Display window", output);
  cv::waitKey(0); // Wait for a keystroke in the window
}

