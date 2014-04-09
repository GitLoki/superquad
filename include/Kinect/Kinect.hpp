#ifndef KINECT_HPP
#define KINECT_HPP

#include <iostream>
#include <vector>
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "../Sensor/Sensor.hpp"

#include "camera.h"

const double PI = 3.14159265;
const int DELAY = 300000/2; //half second delay
const int WIDTH = 640;
const int HEIGHT = 480;
const int THRESHOLD = 1300; // 1.3 meters

//DEFAULT CAPTURE WIDTH AND HEIGHT
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;

//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

//names that will appear at the top of each window
const std::string windowName = "Original Image";
const std::string windowName1 = "HSV Image";
const std::string windowName2 = "Thresholded Image";
const std::string windowName3 = "After Morphological Operations";
const std::string trackbarWindowName = "Trackbars";

class Kinect : public Sensor {

public:
  Kinect();
  virtual ~Kinect();
  virtual bool query(double& x, double& y, double& z);
  void save_video(std::string filename, int frames);
  void save_frame(std::string filename = "snapshot");

private:
  cv::Mat* depthMat;
  cv::Mat* depthf;
  cv::Mat* rgbMat;
  cv::Mat* ownMat;

  bool update();

  bool trackObjects;
  bool useMorphOps;
  Camera camera;

  int rawDepthToMilimeters(int depthValue);
  void on_trackbar( int, void* );
  std::string intToString(int number);
  void createTrackbars();
  void drawObject(int x, int y,cv::Mat &frame);
  void morphOps(cv::Mat &thresh);
  void trackFilteredObject(int &x, int &y, cv::Mat threshold, 
			   cv::Mat &cameraFeed);
  float getrealwidth(float avgX, float depth);
  float getrealheight(float avgY, float depth);

  //Colourtracking
  cv::Mat cameraFeed;
  cv::Mat HSV;
  cv::Mat threshold;
  cv::VideoWriter writer;

};

#endif /* KINECT_HPP */
