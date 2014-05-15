#include "../../include/CascadeControl/PositionControl.hpp"

Location PositionControl::query(Location position) {
	
    Location delta_P = K * (setPoint - position);

    delta_P.limit(shiftLimit);

    currentValue += delta_P;

    if(currentValue.X > 255)
      currentValue.X = 255;
    else if(currentValue.X < 0)
      currentValue.X = 0;
    if(currentValue.Y > 255)
      currentValue.Y = 255;
    else if(currentValue.Y < 0)
      currentValue.Y = 0;
    if(currentValue.Z > 255)
      currentValue.Z = 255;
    else if(currentValue.Z < 0)
      currentValue.Z = 0;

    return currentValue;
}
