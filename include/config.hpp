// for use with usleep
#define ONE_SECOND 1000000

// centre of kinect field of vision
const double XCENTRE = 378;
const double YCENTRE = 263;
const double ZCENTRE = 660;

// frequency of main loop
const int FPS = 100;

// amount of time allowed to put quadcopter in launch cradle and hide under a table
const int COUNTDOWN = 15;

// null location object for testing kinect results against
Location nullLoc(0,0,0);

// set point for velocity calibration
Location v_setPoint(0, 0, 5);

// data objects for velocity control
Location v_K(0.1, -0.1, 1.0);
Location v_snapLimit(snapLimit(5, 5, 15);

// data objects for acceleration control
Location a_K(0.1, 0.1, 0.5);
Location a_jerkLimit(5, 5, 15);
    


