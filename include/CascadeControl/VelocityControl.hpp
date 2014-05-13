#ifndef VELOCITY_CONTROL_HPP
#define VELOCITY_CONTROL_HPP
#include "../datatypes.hpp"

class VelocityControl{
 
private:
    Location K;
    Location currentValue;
    Location setPoint;
    Location snapLimit;

public:
    VelocityControl(Location _K, Location _snapLimit) :
                    K(_K), snapLimit(_snapLimit) {}

  VelocityControl() : K(1.0, 1.0, 15.0), snapLimit(8, 8, 15) {};
    
    void changeSetPoint(Location _setPoint);

    Location query(Location velocity);
};

#endif
