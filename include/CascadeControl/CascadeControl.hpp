#ifndef CASCADE_CONTROL_HPP
#define CASCADE_CONTROL_HPP
#include "../datatypes.hpp"
#include "VelocityControl.hpp"
#include "AccelerationControl.hpp"


class CascadeControl {
private:
	VelocityControl* velocityControl;
	AccelerationControl* accelerationControl;
	Location currentLocation, currentVelocity(0,0,0), oldVelocity, currentAcceleration(0,0,0), oldAcceleration, startSettings;
	double oldTime, newTime, timeInterval;
	int counter = 0;
	int velocityFrequency = 5;

public:
	CascadeControl(Location _startSettings);
	query(Location newLocation);
};

#endif
