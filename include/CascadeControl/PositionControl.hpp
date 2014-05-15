#ifndef POSITION_CONTROL_HPP
#define POSITION_CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"

class PositionControl {

private:

    Location K;
    Location currentValue;
    Location setPoint;
    Location shiftLimit;

public:
    PositionControl(Location _K, Location _shiftLimit) :
      K(_K), shiftLimit(_shiftLimit) {};

    void changeSetPoint(Location _setPoint);

    Location query(Location position);
};

#endif 
// LOC_CONTROL_HPP