#include "../../include/PhysicsModel/PhysicsModel.hpp"

PhysicsModel::PhysicsModel() {
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

  /* Initialise communication thread */
  clientID = simxStart((simxChar*) "127.0.0.1", (simxInt) 19997,
		       (simxChar) true, 
		       (simxChar) true,
		       (simxInt) 3000, (simxInt) 5);

  /* Get object handles */
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
  /*
  thrustStr[0] = '0';
  thrustStr[1] = '\0';
  */

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
  
  /* If lateral movement change Eulers angles */
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
  
  /* Otherwise set position */
  else {

    /*
    err = simxGetObjectPosition(clientID, quadHandle, -1,
				quadPosRead, 
				simx_opmode_oneshot_wait);
    */

    switch(ch){
    case '+':
      thrustVal ++;
	// simxQuery(clientID,"thrust",(simxUChar*) "increase",12,
	//	"reply",replyData,&replySize,5000);
      
      // thrust = thrust <= 8 ? thrust + 1 : 9;
      // quadPosRead[2] -= 0.05;
      break;
    case '-':
      thrustVal = thrustVal > 0 ? thrustVal - 1 : 0;
  //simxQuery(clientID,"thrust",(simxUChar*) "decrease",12,
  //		"reply",replyData,&replySize,5000);
      // thrust = thrust >= 1 ? thrust - 1 : 0;
      // quadPosRead[2] += 0.05;
      break;
    }
    
    simxSetIntegerSignal(clientID,"thrust",thrustVal,simx_opmode_oneshot_wait);

    /*
    switch(thrust) {
    case 0:
    thrustStr[0] = '0';
    break;
    case 1:
    thrustStr[0] = '1';
    break;
    case 2:
    thrustStr[0] = '2';
    break;
    case 3:
    thrustStr[0] = '3';
    break;
    case 4:
    thrustStr[0] = '4';
    break;
    case 5:
    thrustStr[0] = '5';
    break;
    case 6:
    thrustStr[0] = '6';
    break;
    case 7:
    thrustStr[0] = '7';
    break;
    case 8:
    thrustStr[0] = '8';
    break;
    case 9:
    thrustStr[0] = '9';
    break;
    }
    */

    /*
    err = simxSetObjectPosition(clientID, quadHandle, -1,
				quadPosRead,
				simx_opmode_oneshot_wait);
    */
  }
}


/*

    simClearStringSignal("thrust")
    if (req=="increase") then
        simSetStringSignal("reply","52\0") -- will be automatically cleared by the client
		thrustconst = 7
    end
    if (req=="decrease") then
        simSetStringSignal("reply","42\0") -- will be automatically cleared by the client
		thrustconst = 5
*/

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
