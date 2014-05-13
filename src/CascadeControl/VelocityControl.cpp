#include "../../include/CascadeControl/VelocityControl.hpp"

void VelocityControl::changeSetPoint(Location _velocity){
    setPoint = _velocity;
}

Location VelocityControl::query(Location velocity){

    Location delta_V = K * (setPoint - velocity);

    delta_V.limit(snapLimit);

    currentValue += delta_V;

    return currentValue;
}
