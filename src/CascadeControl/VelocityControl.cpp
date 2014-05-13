#include "../../include/CascadeControl/VelocityControl.hpp"

void VelocityControl::changeSetPoint(int _velocity){
    setPoint = velocity_;
}

double VelocityControl::query(double velocity){

    Location delta_V = K * (setpoint - velocity);

    delta_A.limit(snapLimit);

    currentValue += delta_V;

    return currentValue;
}
