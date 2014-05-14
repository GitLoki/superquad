#ifndef ACCEL_CONTROL_HPP
#define ACCEL_CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"

class AccelerationControl {

private:

    Location K;
    Location currentValue;
    Location setPoint;
    Location jerkLimit;

public:
    AccelerationControl(Location _startPoint, Location _K, Location _jerkLimit) :
      K(_K), jerkLimit(_jerkLimit), currentValue(_startPoint) {};

//  currentAcceleration(_currentAcceleration), currentValue(_startPoint)

  AccelerationControl(Location _startPoint): currentValue(_startPoint), K(0.1, 0.1, 0.5), jerkLimit(5, 5, 15) {};
    
    void changeSetPoint(Location _setPoint);

    Location query(Location acceleration);
};

#endif
