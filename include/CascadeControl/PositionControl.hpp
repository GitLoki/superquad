#ifndef POSITION_CONTROL_HPP
#define POSITION_CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"
#include "Control.hpp"

class PositionControl : public Control {

private:
    Location shiftLimit;

public:
    PositionControl(Location _K, Location _shiftLimit) :
        Control(_K), shiftLimit(_shiftLimit) {};

    Location query(Location position);
};

#endif // POSITION_CONTROL_HPP
