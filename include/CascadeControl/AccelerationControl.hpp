#ifndef ACCEL_CONTROL_HPP
#define ACCEL_CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"
#include "Control.hpp"

class AccelerationControl : public Control {

private:
    Location jerkLimit;

public:
    AccelerationControl(Location _startPoint, Location _K, Location _jerkLimit) :
        Control(_k), jerkLimit(_jerkLimit), currentValue(_startPoint) {};

    //currentAcceleration(_currentAcceleration), currentValue(_startPoint)

    //AccelerationControl(Location _startPoint): currentValue(_startPoint), K(0.1, 0.1, 0.5), jerkLimit(5, 5, 15) {};

    Location query(Location acceleration);
};

#endif //ACCEL_CONTROL_HPP
