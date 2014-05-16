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

    Location query(Location velocity);
};

#endif //VELOCITY_CONTROL_HPP
