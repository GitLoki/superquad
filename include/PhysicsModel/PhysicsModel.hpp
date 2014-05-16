#ifndef PHYSICSMODEL_HPP
#define PHYSICSMODEL_HPP

#include <iostream>
#include <cstdlib>

extern "C" {
#include "extApi.h"
#include "extApiCustom.h"
#include "extApiPlatform.h"
}

class PhysicsModel{

public:
	/* Constructor */
	PhysicsModel();

	/* Destructor */
	~PhysicsModel();

	/* Initialises communication with VREP and sets object handles, returns -1 if 
	   there is a communication error, else returns clientID */
	int init();

	/* Starts the simulation running */
	int startSimulation();

	/* Sends the command corresponding to 'ch' to the physics model */
	void sendCommand(char ch);

	simxInt err;     

	// stop the simulation
	void stop();

	// fetch the Euler angles of the drone
	void getRotation(double angles[3]);

	// fetch the position of the drone
	void getPosition(double position[3]);
  
	// rectifies position inside model
	void rectify();

	// sets the location that the quadcopter is heading towards inside the model
	void moveTarget(double coords[3]);

	// sets the position of the quadcopter inside the model
	void setPosition(double position[3]);
  
	double kinX, kinY, kinZ;

private:
	simxInt clientID;
	simxInt errGetHandle[6];
	simxInt quadHandle, targetHandle, propellerRespondable[4];
	simxInt scriptHandle;

	int thrustVal;

	simxFloat* quadPosWrite;
	simxFloat* quadPosRead;
	simxFloat* eulerAnglesRead;
	simxFloat* eulerAnglesWrite;

	double coord_ratio;
};

#endif /* PHYSICSMODEL_HPP */
