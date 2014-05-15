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
const int THRESHOLD = 1600; //in mm

class Kinect
{
private:

    cv::Mat* depthMat;
    cv::Mat* depthf;
    cv::Mat* rgbMat;
    Camera camera;

    //update rgb and depth readings
    void update();

    //conversion of readings to useful mm values
    int rawDepthToMillimeters(int depthValue);
    float getRealWidth(float avgX, float depth);
    float getRealHeight(float avgY, float depth);


public:

    //constructor & destructor
    Kinect();
    ~Kinect();

    //main function - returns location of object
    bool query(double& x, double& y, double& z, cv::Mat* = NULL);
    Location query();

    //functions to save a video or a single frame
    void saveVideo(std::string filename, int frames);
    void saveFrame(std::string filename = "snapshot");
};

#endif /* KINECT_HPP */
