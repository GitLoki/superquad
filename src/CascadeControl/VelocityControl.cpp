#include "../../include/CascadeControl/VelocityControl.hpp"

void VelocityControl::changeSetPoint(int _velocity){
    setPoint = velocity_;
}

double VelocityControl::query(double velocity){
    double delta_V = (K * (setpoint - velocity));
    double acceleration = std::min(snapLimit, std::abs(delta_V));

    acceleration *= (delta_V < 0 ? -1 : 1);

    return acceleration;
}
