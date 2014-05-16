#include "../../include/CascadeControl/VelocityControl.hpp"
#include "../../include/CascadeControl/AccelerationControl.hpp"
#include "../../include/Tx/Tx.hpp"
#include "../../include/Kinect/Kinect.hpp"
#include "../../include/datatypes.hpp"
#include <unistd.h>

// we want to use seconds here for the time
void updateVectors(int time, Location currentLocation, Location lastLocation, Location& currentVelocity, Location& lastVelocity, Location& acceleration) {
    lastVelocity = currentVelocity;
    
    currentVelocity.X = (currentLocation.X - lastLocation.X) / time;
    currentVelocity.Y = (currentLocation.Y - lastLocation.Y) / time;
    currentVelocity.Z = (currentLocation.Z - lastLocation.Z) / time;

    acceleration.X = (currentVelocity.X - lastVelocity.X) / time;
    acceleration.Y = (currentVelocity.Y - lastVelocity.Y) / time;
    acceleration.Z = (currentVelocity.Z - lastVelocity.Z) / time;
}
    
// this program whether tests the cascade control is functioning correctly
int main() {
    Tx* tx = new Tx;
    Kinect* kinect = new Kinect;
    Location currentLocation, 
	     lastLocation,

	     currentVelocity,
	     lastVelocity,

	     acceleration, 
    
	     velocitySetPoint,
	     accelerationSetPoint;

    int currenttime_V, lasttime_V, currenttime_A, lasttime_A;

    VelocityControl* X_VC = new VelocityControl;
    VelocityControl* Y_VC = new VelocityControl;
    VelocityControl* Z_VC = new VelocityControl;

    AccelerationControl* X_AC = new AccelerationControl;
    AccelerationControl* Y_AC = new AccelerationControl;
    AccelerationControl* Z_AC = new AccelerationControl;

    kinect.query(currentLocation.X, currentLocation.Y, currentLocation.Z);
    lastLocation = thisLocation;

    currentVelocity.setValues(0,0,0);
    lastVelocity = currentVelocity;

    velocitySetPoint.setValues(0,0,15); // mm/s
    accelerationSetPoint.setValues(0,0,0); // mm/s/s
    
    acceleration.setValues(0,0,0);
    
    currentTime_V = currentTime_A = clock();

    while (true) {
	
