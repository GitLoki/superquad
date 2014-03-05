#include "../../include/PhysicsModel/PhysicsModel.hpp"

PhysicsModel::PhysicsModel() {
  
  //  simxFloat* quadPosWrite = new simxFloat[3];
  simxFloat* quadPosRead = new simxFloat[3];
  simxFloat* eulerAnglesRead = new simxFloat[3];
  // simxFloat* eulerAnglesWrite = new simxFloat[3];
  
  simxInt clientID = simxStart((simxChar*) "127.0.0.1", (simxInt) 19997,
			       (simxChar) true, 
			       (simxChar) true,
			       (simxInt) 3000, (simxInt) 5);

  if(clientID == -1) std::cout << "ERROR ESTABLISHING CONNECTION TO VREP" << std::endl;
  
  for(int i = 0 ; i < 3 ; i++) {
      quadPosRead[i] = 0;
      // quadPosWrite[i] = 0;
      eulerAnglesRead[i] = 0;
      // eulerAnglesWrite[i] = 0;
  }
  
  /* get handles */

  simxInt errGetHandle[6] = {};
  errGetHandle[0] = simxGetObjectHandle(clientID, (simxChar*) 
					"Quadricopter_base#", &quadHandle, 
					(simxInt) simx_opmode_oneshot_wait);
  errGetHandle[1] = simxGetObjectHandle(clientID, (simxChar*) 
					"Quadricopter_propeller_joint1#", 
					propellerRespondable, 
					(simxInt) simx_opmode_oneshot_wait);
  errGetHandle[2] = simxGetObjectHandle(clientID, (simxChar*) 
					"Quadricopter_propeller_joint2#",
					(propellerRespondable + 1), 
					(simxInt) simx_opmode_oneshot_wait);
  errGetHandle[3] = simxGetObjectHandle(clientID, (simxChar*) 
					"Quadricopter_propeller_joint3#",
					(propellerRespondable + 2), 
					(simxInt) simx_opmode_oneshot_wait);
  errGetHandle[4] = simxGetObjectHandle(clientID, (simxChar*) 
					"Quadricopter_propeller_joint4#",
					(propellerRespondable + 3), 
					(simxInt) simx_opmode_oneshot_wait);
  errGetHandle[5] = simxGetObjectHandle(clientID, (simxChar*) 
					"Quadricopter_target", &targetHandle, 
					simx_opmode_oneshot_wait);
  
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

void PhysicsModel::setRotation(char ch) {
  
  // If lateral movement change Eulers angles
  if(ch != '+' && ch != '-'){
    
    err = simxGetObjectOrientation(clientID, quadHandle, 
				   -1, (simxFloat*) eulerAnglesRead, 
				   simx_opmode_streaming);
    
    switch(ch){
    case 'd': // roll right
      eulerAnglesRead[0] += 10;
      break;
    case 'a': // roll left
      eulerAnglesRead[0] -= 10;
      break;
    case 'w': // pitch forward
      eulerAnglesRead[1] += 10;
      break;
    case 'x': // pitch backward
      eulerAnglesRead[1] -= 10;
      break;
    case 'q': // yaw right
      eulerAnglesRead[2] += 10;
      break;
    case 'e': // yaw left
      eulerAnglesRead[2] -= 10;
      break;
    }     
    
  /*case '+': // thrust increase
    controls[THROTTLE] += 10;
    break;
   case '-': // thrust decrease
   controls[THROTTLE] += 10;
   break;
  */
    err =  simxSetObjectOrientation(clientID, quadHandle, 
				    -1, (simxFloat*) eulerAnglesRead, 
				    simx_opmode_streaming);
  return;
  }
  
  // If height, shift model position vertically
  err = simxGetObjectOrientation(clientID, quadHandle, 
				 -1, (simxFloat*) quadPosRead, 
				 simx_opmode_streaming);

  switch(ch){
  case '+':
    quadPosRead[2] += 10;
    break;
  case '-':
    quadPosRead[2] -= 10;
    break;
  }
 err =  simxSetObjectPosition(clientID, quadHandle, 
			      -1, (simxFloat*)  quadPosRead,
			      simx_opmode_streaming);  
}
