#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "datatypes.hpp"

// for use with usleep
#define ONE_SECOND 1000000

// centre of kinect field of vision
const double XCENTRE = -50;
const double YCENTRE = 5;
const double ZCENTRE = 1000;

// frequency of main loop
const int FPS = 50;

// amount of time allowed to put quadcopter in launch cradle and hide under a table
const int COUNTDOWN = 15;

#endif // CONFIG_HPP
