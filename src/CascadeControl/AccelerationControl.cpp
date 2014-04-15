#include "../../include/CascadeControl/AccelerationControl.hpp"

void AccelerationControl::changeSetPoint(double _setPoint) {
    setPoint = _setPoint;
}

int AccelerationControl::query(double acceleration) { 
    int delta_A = (int) (K * (setpoint - acceleration));
    int delta_Val = std::min(jerkLimit, std::abs(delta_A));

    currentValue += delta_Val * (delta_A < 0 ? -1 : 1);

    return currentValue;
}
