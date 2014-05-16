#include <iostream>
#include <fstream>
#include "../../include/Tx/Tx.hpp"
using namespace std;


//This is an early proof of concept version
//code much improved (and tidied!) in V2

int main (int argc, char** argv) {

  Tx tx;
  char holderChar;
  int controllerCommand[8];
  int lights = 0;
  bool flying = true;

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
      // vertical axis pitch
      else if(controllerCommand[7] == 4){
	//backward
	if(controllerCommand[4] == 1) tx.sendCommand('w');
	//forward
	else if(controllerCommand[4] == -1) tx.sendCommand('x');
	//stop
	else tx.setElevator(137);
      }
      // horizontal axis roll
      else if(controllerCommand[7] == 4){
	//left
	if(controllerCommand[4] == 1) tx.sendCommand('a');
	//right
	else if(controllerCommand[4] == -1) tx.sendCommand('d');
	//stop
	else tx.setAileron(127);
      }
      // 4 turn left
      else if(controllerCommand[7] == 3){
	if(controllerCommand[4] == 1) tx.sendCommand('e');
	else tx.sendCommand('q');
      }
      // 1 turn right
      else if(controllerCommand[7] == 0){
	if(controllerCommand[4] == 1) tx.sendCommand('q');
	else tx.sendCommand('e');
      }
      else continue;
    }
  }
  return 0;
}

      

