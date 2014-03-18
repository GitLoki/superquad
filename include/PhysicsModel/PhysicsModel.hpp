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

  void stop();
  
  void getRotation(double angles[3]);
  
  void getPosition(double position[3]);
  
  void rectify();

  void moveTarget(double coords[3]);

  void setPosition(double position[3]);
  
  double kinX, kinY, kinZ;

private:
  simxInt clientID;
  simxInt errGetHandle[6];
  simxInt quadHandle, targetHandle, propellerRespondable[4];
  simxInt scriptHandle;

  int thrustVal;
  /*
  simxUChar thrustStr[2];
  simxUChar** replyData;
  simxInt replySize;
  */

  simxFloat* quadPosWrite;
  simxFloat* quadPosRead;
  simxFloat* eulerAnglesRead;
  simxFloat* eulerAnglesWrite;

  // TO BE CALCULATED
  double coord_ratio = 1;
};

#endif /* PHYSICSMODEL_HPP */
