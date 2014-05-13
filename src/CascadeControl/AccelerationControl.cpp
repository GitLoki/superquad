#include "../../include/CascadeControl/AccelerationControl.hpp"

void AccelerationControl::changeSetPoint(Location _setPoint) {
    setPoint = _setPoint;
}

Location AccelerationControl::query(Location acceleration) {
	
    Location delta_A = K * (setpoint - acceleration);

    delta_A.limit(jerkLimit);

    currentValue += delta_A;

    return currentValue;
}
