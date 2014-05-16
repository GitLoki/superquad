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
    Control(_K, _startPoint), jerkLimit(_jerkLimit) {};
  
  Location query(Location acceleration);
};

#endif //ACCEL_CONTROL_HPP
