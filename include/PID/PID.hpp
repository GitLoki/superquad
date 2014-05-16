#ifndef PID_HPP
#define PID_HPP

#include "../Kinect/Kinect.hpp"
#include "../Tx/Tx.hpp"
#include "../datatypes.hpp"
#include "../config.hpp"
#include <cmath>
#include <unistd.h>

// stream headers for printing and writing to logfile
#include <iostream>
#include <fstream>

// csignal for catching ctrl-C so that we can close the logfile
#include <csignal>

const int STARTPOW = 180;
const int MINDEPTH = 800;

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
    Location partial_derivatives;
    Location delta;
    Location previous_delta;
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
    void signalHandler(int signum );
};

#endif
