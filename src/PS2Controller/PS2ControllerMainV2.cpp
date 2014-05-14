#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/Tx/Tx.hpp"
using namespace std;


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
      for(int i = 0; i < 8; i++) cout << controllerCommand[i];
      cout << endl;
      // 10 lights
      if(controllerCommand[7] == 8){
	if(controllerCommand[4] == 1){
	lights++;
	tx.setLEDS(lights%2);
	}
      }
      // 9 ABORT
      else if(controllerCommand[7] == 9){
	if(controllerCommand[4] == 1){      
	tx.halt();
	flying = false;
	break;
	}
      }
      // R1 up
      else if(controllerCommand[7] == 6){
	if(controllerCommand[4] == 1) tx.sendCommand('+');
      }
      // R2 down
      else if(controllerCommand[7] == 7){
	  if(controllerCommand[4] == 1) tx.sendCommand('-');
      }
      // Left stick, vertical axis, pitch
      else if(controllerCommand[7] == 1){
	//backward
	if(controllerCommand[4] == 1){
	  // ouput is unsigned 8 bit ints, negative output for high 
	  // pressure, positive for low
	  elevator -= 10 * ((127-abs(controllerCommand[1]))/255);
	}
	//forward
	else if(controllerCommand[4] == -1){
	  elevator += 10 * ((127-abs(controllerCommand[1]))/255);
	}
	//stop
	else{
	  elevator = 137;
	}
	tx.setElevator(elevator);
      }
      // Left stick, horizontal axis roll
      else if(controllerCommand[7] == 0){
	//left
	if(controllerCommand[4] == 1){
	  aileron -= 10 * ((127-abs(controllerCommand[1]))/255);
	}
	//right
	else if(controllerCommand[4] == -1){
	  aileron += 10 * ((127-abs(controllerCommand[1]))/255);
	}
	//stop
	else aileron = 127;
	tx.setAileron(aileron);
      }
      //  Right stick, horizontal axis
      else if(controllerCommand[7] == 3){
	// turn left
	if(controllerCommand[4] == 1){
 	  rudder -= 10 * ((127-abs(controllerCommand[1]))/255);
	}
	// turn right
	else if(controllerCommand[4] == 1){
	  rudder += 10 * ((127-abs(controllerCommand[1]))/255);	
	}
	else rudder = 137;
	tx.setRudder(rudder);
      }
      else continue;
    }
  }
  return 0;
}

      

