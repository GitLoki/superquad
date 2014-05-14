#include "../../include/CascadeControl/CascadeControl.hpp"

CascadeControl::CascadeControl(Location _startSettings, Location v_snapLimit, Location v_K,
                               Location a_jerkLimit, Location a_K) {
	accelerationControl = new AccelerationControl(_startSettings, a_K, a_jerkLimit);
	velocityControl = new VelocityControl(v_K, v_snapLimit);
	startSettings = _startSettings;
}

CascadeControl::~CascadeControl() {
	delete accelerationControl;
	delete velocityControl;
}

Location CascadeControl::query(Location newLocation, Location v_snapLimit) {
	oldLocation = currentLocation;
	currentLocation = newLocation;

	if (!counter) {
	        oldTime = clock();
		currentLocation = newLocation;
		counter++;
		oldVelocity = currentVelocity;
		oldAcceleration = currentAcceleration;
		return startSettings;
	}
	
	// calculate time since last query
	newTime = clock();
	timeInterval = (newTime - oldTime) / CLOCKS_PER_SEC;
	oldTime = newTime;


	// calculate new velocity and acceleration
	deltaL = currentLocation - oldLocation;
	currentVelocity = (0.5 * currentVelocity) + (0.5 * deltaL / timeInterval);
	deltaV = currentVelocity - oldVelocity;
	currentAcceleration = (0.5 * currentAcceleration) + (0.5 * deltaV / timeInterval);
	std::cout << "Velocity: " << currentVelocity << std::endl
	          << "Acceleration: " << currentAcceleration << std::endl;


	if (counter % velocityFrequency == 0) {
	         accelerationSetPoint = velocityControl->query(currentVelocity, v_snapLimit);
		accelerationControl->changeSetPoint(accelerationSetPoint);
		counter = 0;
		std::cout << "Acceleration setpoint: " << accelerationSetPoint << std::endl;
	}

	counter++;

	currentSettings = accelerationControl->query(currentAcceleration);

	return currentSettings;
}

void CascadeControl::changeVelocitySetPoint(Location newSetPoint) {
  velocityControl->changeSetPoint(newSetPoint);
}
