#ifndef VELOCITY_CONTROL_HPP
#define VELOCITY_CONTROL_HPP
#include "../datatypes.hpp"

class VelocityControl{
 
private:
 
 	Location K = new Location(5.0, 5.0, 5.0);
    Location currentValue = new Location(0, 0, 0);
    Location setPoint = new Location(0, 0, 0);
    Location snapLimit = new Location(15, 15, 15);

public:
    VelocityControl(Location _K, Location _currentValue, Location _snapLimit) :
	                    K(_K), currentVelocity(_currentVelocity), snapLimit(_snapLimit) {}

    VelocityControl() {};
    
    void changeSetPoint(Location _setPoint);

    int query(Location velocity);
};

#endif
