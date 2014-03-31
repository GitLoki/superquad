#ifndef PID_HPP
#define PID_HPP

#include "../Kinect/Kinect.hpp"
#include "../Tx/Tx.hpp"
#include <cmath>
#include <unistd.h>
class PID {
private:
    const double XCENTRE = 250;
    const double YCENTRE = 200;
    const double ZCENTRE = 70;
    int control_vals[4];
    double destination[3];
    double location[3];
    double ratios[3]; 
    Kinect* kinect;
    Tx* tx;

public:
    PID(Kinect* kinect, Tx* tx);
    ~PID(){};
    int getLocation(double* _location);
    void setDestination(double* _destination);
    void query();
    void goToDestination() {
}

#endif