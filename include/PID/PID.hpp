#ifndef PID_HPP
#define PID_HPP

#include "../Kinect/Kinect.hpp"
#include "../Tx/Tx.hpp"
#include "../datatypes.hpp"
#include <cmath>
#include <unistd.h>

// stream headers for printing and writing to logfile
#include <iostream>
#include <fstream>

// csignal for catching ctrl-C so that we can close the logfile
#include <csignal>

const int STARTPOW = 180;
const int MINDEPTH = 800;

const int FPS = 100;
const int COUNTDOWN = 15;

const double XCENTRE = 240;
const double YCENTRE = 320;
const double ZCENTRE = 1000;

std::ofstream logfile;

class PID {
private:

    int trim[4];
    int control_vals[4];
    Location destination;
    Location location;
    Location ratios; 
    Kinect* kinect;
    Tx* tx;
    int updateLocation();
    int updateRatios();

public:
    PID(Kinect* kinect, Tx* tx);
    ~PID(){};
    void updateDestination(Location* _destination);
    int goToDestination(Location& _currentLocation);
    void halt();
};

#endif
