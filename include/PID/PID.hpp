#ifndef PID_HPP
#define PID_HPP

#include "../Kinect/Kinect.hpp"
#include "../Tx/Tx.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>

const int STARTPOW = 170;

class PID {
private:
    const double XCENTRE = 240;
    const double YCENTRE = 320;
    const double ZCENTRE = 70;
    int control_vals[5];
    double destination[3];
    double location[3];
    double ratios[3]; 
    Kinect* kinect;
    Tx* tx;

public:
    PID(Kinect* kinect, Tx* tx);
    ~PID(){};
    int updateLocation();
    void setDestination(double* _destination);
    void update();
    void goToDestination();
};

#endif
