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


class PID {
private:
    int STARTPOW;
    int MINDEPTH;
    int MAXTHROTTLE;
    double KP_lateral;
    double KI_lateral;
    double KD_lateral;
    double KP_x;
    double KP_y;
    double KP_z;
    double KI_x;
    double KI_y;
    double KI_z;
    double KD_x;
    double KD_y;
    double KD_z;
    
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
    // PID(Kinect* kinect, Tx* tx);
    PID(Kinect* _kinect, Tx* _tx, 
	int _KP_lateral, int _KP_z,
	int _KI_lateral, int _KI_z, 
	int _KD_lateral, int _KD_z,
	int _STARTPOW, int _MINDEPTH);
    ~PID(){};
    void updateDestination(Location* _destination);
    int goToDestination(Location& _currentLocation);
    void halt();
    void signalHandler(int signum );
};

#endif
