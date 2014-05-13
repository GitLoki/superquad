#ifndef KINECT_HPP
#define KINECT_HPP

#include <iostream>
#include <vector>
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "camera.h"
#include "../../include/datatypes.hpp"

const double PI = 3.14159265;
const int WIDTH = 640;
const int HEIGHT = 480;
const int THRESHOLD = 2000; // 1.3 meters

class Kinect {

public:
  Kinect();
  ~Kinect();
  bool query(double& x, double& y, double& z);
  Location query();
  void save_video(std::string filename, int frames);
  void save_frame(std::string filename = "snapshot");
  float getrealwidth(float avgX, float depth);
  float getrealheight(float avgY, float depth);

private:
  void update();
  int rawDepthToMilimeters(int depthValue);

  cv::Mat* depthMat;
  cv::Mat* depthf;
  cv::Mat* rgbMat;
  Camera camera;
};

#endif /* KINECT_HPP */
