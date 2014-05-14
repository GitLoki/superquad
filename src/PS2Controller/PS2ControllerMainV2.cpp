#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/Tx/Tx.hpp"
using namespace std;

//MAKE SURE ANALOG IS LIGHT UP!!!!!

int main (int argc, char** argv) {

  Tx tx;
  char holderChar;
  int controllerCommand[8];
  int lights = 1;
  bool flying = true;
  int aileron = 127;
  int elevator = 137;
  int rudder = 137;  

  ifstream in_stream;
  in_stream.open("/dev/input/js0");
  tx.setThrust(80);

  // first 9x16 chars are junk
  for(int i = 0; i!=144; i++){ 
    in_stream.get(holderChar);
  }

  /* Main loop of program */
  while(flying){
    if(!in_stream.eof()){
      for(int i = 0; i < 8; i++){
	in_stream.get(holderChar);
	controllerCommand[i] = (int)holderChar;
      }
      switch(controllerCommand[7]){
	// 10 lights
      case 8:
	if(controllerCommand[4] == 1){
	  lights++;
	  tx.setLEDS(lights%2);
	}
	break;
      case 9:
	// 9 ABORT
	if(controllerCommand[4] == 1){      
	  tx.halt();
	  flying = false;
	}
	break;
      case 6:
	//R1 up
	if(controllerCommand[4] == 1) tx.sendCommand('+');
	
	// R2 down
      case 7:
	if(controllerCommand[4] == 1) tx.sendCommand('-');
       	break;
	// Left stick, vertical axis, pitch
      case 1:
	//stop
	if(controllerCommand[5] == 0) elevator = 137;
	//forward
	else if(controllerCommand[5] > 0) elevator = 137 + (10*((controllerCommand[5]/127)*100));
	//backwards
	else elevator = 137 + (10*((controllerCommand[5]/128)*100));
	tx.setElevator(elevator);
	break;
	// Left stick, horizontal axis roll
      case 0:
	if(controllerCommand[6] == 2){    //left stick
	//stop
	  if(controllerCommand[5] == 0) aileron = 127;
	//left 
	  if(controllerCommand[5] < 0) aileron = 127 + (10*((controllerCommand[5]/128)*100));
	//right
	  else  aileron += 127 + (10*((controllerCommand[5]/127)*100));
	  tx.setAileron(aileron);
	  break;
	}
	else{                           //right stick
	  //stop 
	  if(controllerCommand[5] == 0) rudder = 137;
	  //turn right
	else rudder = 137 + (10*((controllerCommand[5]/127)*100));
	  tx.setRudder(rudder);
	  break;
	}
	// Right stick, horizontal axis
      case 3:
	//stop 
	if(controllerCommand[5] == 0) rudder = 137;
	// turn left
	else rudder = 137 + (10*((controllerCommand[5]/128)*100));
	tx.setRudder(rudder);
	break;

      default: continue;
      }  
    }
  }
  return 0;
}
  
      

