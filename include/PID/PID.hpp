#ifndef PID_HPP
#define PID_HPP

#include "../Sensor/Sensor.hpp"
#include "../Actuator/Actuator.hpp"
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

const double XCENTRE = 378;
const double YCENTRE = 263;
const double ZCENTRE = 660;

std::ofstream logfile;

class PID {
private:
    double previous_time;
    double current_time;
    double time_diff;
    int trim[4];
    int control_vals[4];
    Location destination;
    Location location;
    Location ratios; 
    Location integrals;
    Location delta;
    Sensor* kinect;
    Actuator* tx;
    int updateLocation();
    int updateRatios();

public:
    PID(Sensor* kinect, Actuator* tx);
    ~PID(){};
    void updateDestination(Location* _destination);
    int goToDestination(Location& _currentLocation);
    void halt();
};

#endif
