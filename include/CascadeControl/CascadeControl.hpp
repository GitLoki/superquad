#ifndef CASCADE_CONTROL_HPP
#define CASCADE_CONTROL_HPP
#include "../datatypes.hpp"
#include "VelocityControl.hpp"
#include "AccelerationControl.hpp"
#include <ctime>

class CascadeControl {
private:
	VelocityControl* velocityControl;
	AccelerationControl* accelerationControl;

        Location currentLocation, oldLocation, 
	  currentVelocity, oldVelocity, 
	  currentAcceleration, oldAcceleration, 
	  startSettings, currentSettings,
	  deltaL, deltaV, 
	  velocitySetPoint,
	  accelerationSetPoint;

	double oldTime, newTime, timeInterval;
	int counter = 0;
	int velocityFrequency = 10;

public:
	CascadeControl(Location _startSettings);
	Location query(Location newLocation);
        void changeSetPoint(Location newSetPoint);
};

#endif
