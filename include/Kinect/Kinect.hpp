#ifndef KINECT_HPP
#define KINECT_HPP

#include <iostream>
#include <vector>
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "camera.h"

const double PI = 3.14159265;
const int WIDTH = 640;
const int HEIGHT = 480;
const int THRESHOLD = 2000; // 1.3 meters

// image filtering attributes
const int WAIT = 10;
const int LONG_WAIT = 4000;

class Kinect {

public:
  Kinect();
  ~Kinect();
  bool query(double& x, double& y, double& z);
  void save_video(std::string filename, int frames);
  void save_frame(std::string filename = "snapshot");
  float getrealwidth(float avgX, float depth);
  float getrealheight(float avgY, float depth);
  bool filtered_query(double& realX, double& realY, double& avgDepth);

private:
  void update();
  int rawDepthToMilimeters(int depthValue);
  double rawDepthToMilimetersDouble(int depthValue);

  // image filtering
  void show(Mat* image, const std::string str_text, int wait_time);
  void closed_sequence();
  void non_closed_sequence();

  cv::Mat* depthMat;
  cv::Mat* depthf;
  cv::Mat* rgbMat;
  Camera camera;
};

#endif /* KINECT_HPP */
