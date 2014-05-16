#include "../../include/PhysicsModel/PhysicsModel.hpp"

PhysicsModel::PhysicsModel() : coord_ratio(1.0) {
  quadPosWrite = new simxFloat[3];
  quadPosRead = new simxFloat[3];
  eulerAnglesRead = new simxFloat[3];
  eulerAnglesWrite = new simxFloat[3];

  for(int i = 0 ; i < 3 ; i++) {
      quadPosRead[i] = 0;
      quadPosWrite[i] = 0;
      eulerAnglesRead[i] = 0;
      eulerAnglesWrite[i] = 0;
  }
}

PhysicsModel::~PhysicsModel() {
  stop();
}

int PhysicsModel::init() {

  // initialise communication thread 
  clientID = simxStart((simxChar*) "127.0.0.1", (simxInt) 19997,
		       (simxChar) true, 
		       (simxChar) true,
		       (simxInt) 3000, (simxInt) 5);

  // get object handles
  err = simxGetObjectHandle(clientID, (simxChar*) 
			    "Quadricopter_base#", &quadHandle, 
			    (simxInt) simx_opmode_oneshot_wait);
  err = simxGetObjectHandle(clientID, (simxChar*) 
			    "Quadricopter_propeller_joint1#", 
			    propellerRespondable, 
			    (simxInt) simx_opmode_oneshot_wait);
  err = simxGetObjectHandle(clientID, (simxChar*) 
			    "Quadricopter_propeller_joint2#",
			    (propellerRespondable + 1), 
			    (simxInt) simx_opmode_oneshot_wait);
  err = simxGetObjectHandle(clientID, (simxChar*) 
			    "Quadricopter_propeller_joint3#",
			    (propellerRespondable + 2), 
			    (simxInt) simx_opmode_oneshot_wait);
  err = simxGetObjectHandle(clientID, (simxChar*) 
			    "Quadricopter_propeller_joint4#",
			    (propellerRespondable + 3), 
			    (simxInt) simx_opmode_oneshot_wait);
  err = simxGetObjectHandle(clientID, (simxChar*) 
			    "Quadricopter_target", &targetHandle, 
			    simx_opmode_oneshot_wait);

  thrustVal = 0;

  // tell model to begin
  startSimulation();

  return clientID;
}

int PhysicsModel::startSimulation() {
  return simxStartSimulation(clientID,(simxInt) simx_opmode_oneshot);
}

void PhysicsModel::moveTarget(double coords[3]) {
  
  
  quadPosWrite[0] += coords[0];
  quadPosWrite[1] += coords[1];
  quadPosWrite[2] += coords[2];
  
  err =  simxSetObjectPosition(clientID, targetHandle,
			       (simxInt) sim_handle_parent, 
			       quadPosWrite,
			       simx_opmode_oneshot_wait);
  
}

void PhysicsModel::stop() {
  err = simxStopSimulation(clientID, 
			   (simxInt) simx_opmode_oneshot_wait);
}

void PhysicsModel::getRotation(double angles[3]) {
  err = simxGetObjectOrientation(clientID, quadHandle, -1,
				 eulerAnglesRead, 
				 simx_opmode_streaming);
  angles[0] = static_cast<double>(eulerAnglesRead[0]);
  angles[1] = static_cast<double>(eulerAnglesRead[1]);
  angles[2] = static_cast<double>(eulerAnglesRead[2]);
}

void PhysicsModel::getPosition(double position[3]) {
  err = simxGetObjectPosition(clientID, quadHandle, 
			      -1, quadPosRead, 
			      simx_opmode_streaming);
  
  position[0] = static_cast<double>(quadPosRead[0]);
  position[1] = static_cast<double>(quadPosRead[1]);
  position[2] = static_cast<double>(quadPosRead[2]);
}

void PhysicsModel::setPosition(double position[3]) {
  err =  simxSetObjectPosition(clientID, quadHandle, 
			       -1, (simxFloat*) position, 
			       simx_opmode_streaming);
}

void PhysicsModel::sendCommand(char ch) {
  
  // if lateral movement then change Euler angles...
  if(ch != '+' && ch != '-'){
    
    err = simxGetObjectOrientation(clientID, quadHandle, -1,
				   eulerAnglesRead,
				   simx_opmode_oneshot_wait);

    switch(ch){
    case 'd': // roll right
      eulerAnglesRead[0] += 0.07;
      break;
    case 'a': // roll left
      eulerAnglesRead[0] -= 0.07;
      break;
    case 'w': // pitch forward
      eulerAnglesRead[1] += 0.07;
      break;
    case 'x': // pitch backward
      eulerAnglesRead[1] -= 0.07;
      break;
    case 'q': // yaw right
      eulerAnglesRead[2] += 0.07;
      break;
    case 'e': // yaw left
      eulerAnglesRead[2] -= 0.07;
      break;
    }     

    err = simxSetObjectOrientation(clientID, quadHandle, -1,
				   eulerAnglesRead, 
				   simx_opmode_oneshot_wait);
    return;
  }
  
  // ...otherwise set position
  else {
    switch(ch){
    case '+':
      thrustVal++;
      break;
    case '-':
      thrustVal = thrustVal > 0 ? thrustVal - 1 : 0;
      break;
    }

    simxSetIntegerSignal(clientID,"thrust",thrustVal,simx_opmode_oneshot_wait);
  }
}


void PhysicsModel::rectify(){
  err = simxGetObjectPosition(clientID, quadHandle, -1,
			      quadPosRead, 
			      simx_opmode_oneshot_wait);
  
  // If kinect calls itself as the origin we can simply shift the vrep
  // coords by a constant

  // RATIO TO BE CALCULATED, will convert between kinect and vrep
  // coordinate values
  kinX *= coord_ratio;
  kinY *= coord_ratio; 
  kinZ *= coord_ratio; 
  quadPosRead[0] += kinX-quadPosRead[0];
  quadPosRead[1] += kinY-quadPosRead[1];
  quadPosRead[2] += kinZ-quadPosRead[2]; 

  err = simxSetObjectPosition(clientID, quadHandle, -1,
			      quadPosRead,
			      simx_opmode_oneshot_wait);
}
