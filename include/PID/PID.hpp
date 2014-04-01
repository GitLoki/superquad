#ifndef PID_HPP
#define PID_HPP

#include "../Kinect/Kinect.hpp"
#include "../Tx/Tx.hpp"
#include "../datatypes.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>

const int STARTPOW = 170;
const double XCENTRE = 240;
const double YCENTRE = 320;
const double ZCENTRE = 70;
    
class PID {
private:
    int control_vals[5];
    Location destination;
    Location location;
    Location ratios; 
    Kinect* kinect;
    Tx* tx;
    int updateLocation();
    void updateRatios();

public:
    PID(Kinect* kinect, Tx* tx);
    ~PID(){};
    void updateDestination(Location* _destination);
    void goToDestination(Location& _currentLocation);
};

#endif
