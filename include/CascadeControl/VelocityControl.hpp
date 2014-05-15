#ifndef VELOCITY_CONTROL_HPP
#define VELOCITY_CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"
#include "Control.hpp"

class VelocityControl : public Control {
 
private:
    Location snapLimit;

public:
    VelocityControl(Location _K, Location _snapLimit) :
        Control(_K), snapLimit(_snapLimit) {}

    //VelocityControl() : K(0.1, -0.1, 1.0), snapLimit(5, 5, 15) {};

    Location query(Location velocity);
};

#endif //VELOCITY_CONTROL_HPP
