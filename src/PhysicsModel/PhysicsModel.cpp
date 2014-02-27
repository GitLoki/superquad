#include "vrep.h"

Model::Model(){
  
  simxFloat* quadPosWrite = new simxFloat[3];
  simxFloat* quadPosRead = new simxFloat[3];
  simxFloat* eulerAnglesRead = new simxFloat[3];
  simxFloat* eulerAnglesWrite = new simxFloat[3];
  
  //FLY MY PRETTY!
  simxInt clientID = simxStart((simxChar*) "127.0.0.1", (simxInt) 19997,
			       (simxChar) true, 
			       (simxChar) true,
			       (simxInt) 3000, (simxInt) 5);

  if(clientID == -1) cout << "ERROR ESTABLISHING CONNECTION TO VREP" << endl;
  
  for(int i = 0 ; i < 3 ; i++)
    {
      quadPosRead[i] = 0;
      quadPosWrite[i] = 0;
      eulerAnglesRead[i] = 0;
      eulerAnglesWrite[i] = 0;
    }
  
  //Get handles
 simxInt errGetHandle[6] = {};
 errGetHandle[0] = simxGetObjectHandle(clientID, (simxChar*) "Quadricopter_base#",
				       &quadHandle, (simxInt) simx_opmode_oneshot_wait);
 errGetHandle[1] = simxGetObjectHandle(clientID, (simxChar*) "Quadricopter_propeller_joint1#",
				       propellerRespondable, (simxInt) simx_opmode_oneshot_wait);
 errGetHandle[2] = simxGetObjectHandle(clientID, (simxChar*) "Quadricopter_propeller_joint2#",
				       (propellerRespondable + 1), (simxInt) simx_opmode_oneshot_wait);
 errGetHandle[3] = simxGetObjectHandle(clientID, (simxChar*) "Quadricopter_propeller_joint3#",
				       (propellerRespondable + 2), (simxInt) simx_opmode_oneshot_wait);
 errGetHandle[4] = simxGetObjectHandle(clientID, (simxChar*) "Quadricopter_propeller_joint4#",
				       (propellerRespondable + 3), (simxInt) simx_opmode_oneshot_wait);
 errGetHandle[5] = simxGetObjectHandle(clientID, (simxChar*) "Quadricopter_target", &targetHandle, simx_opmode_oneshot_wait);
 
}


void Model::moveTarget(double coords[3]){
  quadPosWrite[0] += coords[0];
  quadPosWrite[1] += coords[1];
  quadPosWrite[2] += coords[2];
  
  simxInt errSetPosition = simxSetObjectPosition(clientID, targetHandle,
						 (simxInt) sim_handle_parent, quadPosWrite,
						 simx_opmode_oneshot_wait);
  
}

void Model::stop(){
  simxInt errStop =  simxStopSimulation(clientID, 
					(simxInt) simx_opmode_oneshot_wait);
}

//TODO EULER MOVEMENT
void Model::getEulers(double eulerAngles[3]){
  simxInt errGetRotation = simxGetObjectOrientation(clientID, quadHandle, -1,
						    eulerAnglesRead, simx_opmode_streaming);
  eulerAngles[0] = static_cast<double>(eulerAnglesRead[0]);
  eulerAngles[1] = static_cast<double>(eulerAnglesRead[1]);
  eulerAngles[2] = static_cast<double>(eulerAnglesRead[2]);
}

void Model::getPosition(double position[3]){
  simxInt errGetPosition = simxGetObjectPosition(clientID, quadHandle, 
						 -1, quadPosRead, 
						 simx_opmode_streaming);
  
  position[0] = static_cast<double>(quadPosRead[0]);
  position[1] = static_cast<double>(quadPosRead[1]);
  position[2] = static_cast<double>(quadPosRead[2]);
}
