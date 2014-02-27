#ifndef VREP_H
#define VREP_H

#include <iostream>
#include <cstdlib>
using namespace std;

extern "C" {
#include "extApi.h"
#include "extApiCustom.h"
#include "extApiPlatform.h"
}

class Model{
  
 private:
  
  simxInt errGetHandle[6];
  simxInt quadHandle, targetHandle, propellerRespondable[4];
  
  simxFloat* quadPosWrite;
  simxFloat* quadPosRead;
  simxFloat* eulerAnglesRead;
  simxFloat* eulerAnglesWrite;
  
  simxInt clientID;
  
 public:
  
  Model();
  
  void moveTarget(double coords[3]);
  
  void stop();
  
  void getEulers(double eulerAngles[3]);

  void getPosition(double position[3]);

};
#endif
