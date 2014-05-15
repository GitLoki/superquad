#ifndef CASCADE_CONTROL_HPP
#define CASCADE_CONTROL_HPP
#include "../datatypes.hpp"
#include "../config.hpp"
#include "VelocityControl.hpp"
#include "AccelerationControl.hpp"
#include "PositionControl.hpp"
#include <ctime>

class CascadeControl {
private:
	PositionControl* positionControl;
	VelocityControl* velocityControl;
	AccelerationControl* accelerationControl;

    Location currentPosition, oldPosition, 
	  currentVelocity, oldVelocity, 
	  currentAcceleration, oldAcceleration, 
	  startSettings, currentSettings,
          deltaP, deltaV, deltaL, 
	  velocitySetPoint,
          accelerationSetPoint;

	double oldTime, newTime, timeInterval;
	int counter = 0,
	    velocityFrequency = 5,
	    positionFrequency = 10;

public:
    CascadeControl(Location _startSettings, 
		Location p_shiftLimit, Location p_K, 
		Location v_snapLimit, Location v_K,
	    Location a_jerkLimit, Location a_K);
	~CascadeControl();
  	Location query(Location newPosition, Location v_snapLimit);
    void changePositionSetPoint(Location newSetPoint);
    void changeVelocitySetPoint(Location newSetPoint);
};

#endif
