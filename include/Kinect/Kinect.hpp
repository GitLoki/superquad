#ifndef KINECT_HPP
#define KINECT_HPP

#include "libfreenect.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <string>

#include <opencv/cv.h>
#include <cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

#include "myfreenectdevice.h"

const double PI = 3.14159265;
const int DELAY = 300000/2; //half second delay
const int WIDTH = 640;
const int HEIGHT = 480;
const int THRESHOLD = 1600; // 1.5 meters

//DEFAULT CAPTURE WIDTH AND HEIGHT
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;

//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

//names that will appear at the top of each window
const std::string windowName = "Original Image";
const std::string windowName1 = "HSV Image";
const std::string windowName2 = "Thresholded Image";
const std::string windowName3 = "After Morphological Operations";
const std::string trackbarWindowName = "Trackbars";

/*******************************/
/*        class: Kinect        */
/*******************************/

class Kinect {

public:

  /* constructor */
  Kinect();

  /* destructor */
  ~Kinect();

  void init();

  int query(double& x, double& y, double& z, double& p);

private:

  cv::Mat* depthMat;
  cv::Mat* depthf;
  cv::Mat* rgbMat;
  cv::Mat* ownMat;

  bool trackObjects = true;
  bool useMorphOps = true;
  

  Freenect::Freenect freenect;
  MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);

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
  //Matrix to store each frame of the webcam feed
  cv::Mat cameraFeed;
  //matrix storage for HSV image
  cv::Mat HSV;
  //matrix storage for binary threshold image
  cv::Mat threshold;
  //video capture object to acquire webcam feed
  cv::VideoCapture capture;



};

#endif /* KINECT_HPP */
