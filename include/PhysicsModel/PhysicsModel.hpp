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
  /* constructor */
  PhysicsModel();

  /* destructor */
  ~PhysicsModel();

  void moveTarget(double coords[3]);
  
  void stop();
  
  void getRotation(double angles[3]);
  
  void getPosition(double position[3]);

  void setRotation(char ch);

  void setPosition(double position[3]);
  
private:
  simxInt err;     //Annoying error container 
                   //used to avoid 'unused variable' warnings  

  simxInt errGetHandle[6];
  simxInt quadHandle, targetHandle, propellerRespondable[4];
  
  simxFloat* quadPosWrite;
  simxFloat* quadPosRead;
  simxFloat* eulerAnglesRead;
  simxFloat* eulerAnglesWrite;
  
  simxInt clientID;
};

#endif /* PHYSICSMODEL_HPP */
