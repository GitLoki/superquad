#include "../../include/CascadeControl/CascadeControl.hpp"

CascadeControl::CascadeControl(Location _startSettings) {
	accelerationControl = new accelerationControl(_startSettings);
	velocityControl = new velocityControl;
	startSettings = _startSettings;
}

Location CascadeControl::query(Location newLocation) {
	oldLocation = currentLocation;
	currentLocation = newLocation;

	if (!counter) {
		oldTime = clock;
		currentLocation = newLocation;
		counter++;
		oldVelocity = currentVelocity;
		oldAcceleration = currentAcceleration;
		return startSettings;
	}
	
	// calculate time since last query
	newTime = clock;
	timeInterval = newTime - oldTime;
	oldTime = newTime;

	// calculate new velocity and acceleration
	deltaL = currentLocation - oldLocation;
	currentVelocity = deltaL / timeInterval;
	deltaV = currentVelocity - oldVelocity;
	currentAcceleration = deltaV / timeInterval;


	if (counter % velocityFrequency == 0) {
		accelerationSetPoint = velocityControl.query(currentVelocity);
		accelerationControl.changeSetPoint(accelerationSetPoint);
		counter = 0;
	}

	counter++;

	return accelerationControl.query(currentAcceleration);
}