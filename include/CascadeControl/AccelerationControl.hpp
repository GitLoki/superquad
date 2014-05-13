#ifndef ACCEL_CONTROL_HPP
#define ACCEL_CONTROL_HPP
#include "../datatypes.hpp"

class AccelerationControl {

private:

    Location K = new Location(5.0, 5.0, 5.0);
    Location currentValue = new Location(0, 0, 0);
    Location setPoint = new Location(0, 0, 0);
    Location jerkLimit = new Location(15, 15, 15);

public:
    AccelerationControl(Location _startPoint, Location _K, Location _currentValue, Location _jerkLimit) :
	                    K(_K), currentAcceleration(_currentAcceleration), jerkLimit(_jerkLimit), currentValue(_startPoint) {}

    AccelerationControl(Location _startPoint): currentValue(_startPoint) {};
    
    void changeSetPoint(Location _setPoint);

    int query(Location acceleration);
};

#endif
