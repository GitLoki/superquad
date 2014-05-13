#ifndef ACCEL_CONTROL_HPP
#define ACCEL_CONTROL_HPP
#include "../datatypes.hpp"

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

  AccelerationControl(Location _startPoint): currentValue(_startPoint), K(5.0, 5.0, 5.0), jerkLimit(15, 15, 15) {};
    
    void changeSetPoint(Location _setPoint);

    Location query(Location acceleration);
};

#endif
