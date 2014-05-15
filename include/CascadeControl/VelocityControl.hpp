#ifndef VELOCITY_CONTROL_HPP
#define VELOCITY_CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"

class VelocityControl{
 
private:
    Location K;
    Location currentValue;
    Location setPoint;
    Location snapLimit;

public:
    VelocityControl(Location _K, Location _snapLimit) :
                    K(_K), snapLimit(_snapLimit) {}

    //VelocityControl() : K(0.1, -0.1, 1.0), snapLimit(5, 5, 15) {};
    
    void changeSetPoint(Location _setPoint);

  Location query(Location velocity);
};

#endif
