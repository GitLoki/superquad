#include "../../include/CascadeControl/AccelerationControl.hpp"

Location AccelerationControl::query(Location acceleration) {
	
    Location delta_A = K * (setPoint - acceleration);

    delta_A.limit(jerkLimit);

    currentValue += delta_A;

	// make sure only sensible values are sent to the transmitter
    if(currentValue.X > 255)
      currentValue.X = 255;
    else if(currentValue.X < 0)
      currentValue.X = 0;
    if(currentValue.Y > 255)
      currentValue.Y = 255;
    else if(currentValue.Y < 0)
      currentValue.Y = 0;
    if(currentValue.Z > 190)
      currentValue.Z = 190;
    else if(currentValue.Z < 0)
      currentValue.Z = 0;

    return currentValue;
}
