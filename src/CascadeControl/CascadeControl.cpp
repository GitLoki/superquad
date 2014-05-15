#include "../../include/CascadeControl/CascadeControl.hpp"

CascadeControl::CascadeControl(Location _startSettings, 
	    Location p_shiftLimit, Location p_K, 
	    Location v_snapLimit, Location v_K,
	    Location a_jerkLimit, Location a_K) {
	accelerationControl = new AccelerationControl(_startSettings, a_K, a_jerkLimit);
	velocityControl = new VelocityControl(v_K, v_snapLimit);
	positionControl = new PositionControl(p_K, p_shiftLimit);
	startSettings = _startSettings;
}

CascadeControl::~CascadeControl() {
	delete accelerationControl;
	delete velocityControl;
	delete positionControl;
}

Location CascadeControl::query(Location newPosition, Location v_snapLimit) {
        oldPosition = currentPosition;
        currentPosition = newPosition;

	if (!counter) {
	    oldTime = clock();
		currentPosition = newPosition;
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
	deltaP = currentPosition - oldPosition;
	currentVelocity = (0.5 * currentVelocity) + (0.5 * deltaL / timeInterval);
	deltaV = currentVelocity - oldVelocity;
	currentAcceleration = (0.5 * currentAcceleration) + (0.5 * deltaV / timeInterval);
	std::cout << "Velocity: " << currentVelocity << std::endl
	          << "Acceleration: " << currentAcceleration << std::endl;

	if (counter % positionFrequency == 0) {
	        velocitySetPoint = positionControl->query(currentPosition);
		velocityControl->changeSetPoint(velocitySetPoint);
		counter = 0;
		std::cout << "Velocity setpoint: " << velocitySetPoint << std::endl;
	}

	if (counter % velocityFrequency == 0) {
	         accelerationSetPoint = velocityControl->query(currentVelocity);
		accelerationControl->changeSetPoint(accelerationSetPoint);
		std::cout << "Acceleration setpoint: " << accelerationSetPoint << std::endl;
	}

	currentSettings = accelerationControl->query(currentAcceleration);

	counter++;

	return currentSettings;
}

void CascadeControl::changeVelocitySetPoint(Location newSetPoint) {
  velocityControl->changeSetPoint(newSetPoint);
}

void CascadeControl::changePositionSetPoint(Location newSetPoint) {
  positionControl->changeSetPoint(newSetPoint);
}
