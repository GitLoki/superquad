#include <iostream>
#include <fstream>
#include "../../include/Tx/Tx.hpp"
using namespace std;

char getArdCommand(int controllerCommand){
  switch(controllerCommand){
  case 0: return 'd';
  case 1: return 'x';
  case 2: return 'w';
  case 3: return 'a';
  case 4: return '-';
  case 5: return 'e';
  case 6: return '+';
  case 7: return 'q';
  case 8: return 'l';
  case 9:  return '!'; 
    //unknown key resets orientation
  default: return ' ';
  }
}

int main (int argc, char** argv) {

  Tx tx;
  char holderChar;
  int controllerCommand;
  char ardCommand;
  int lights = 0;
  bool flying = true;

  ifstream in_stream;
  in_stream.open("/dev/input/js0");
  tx.setThrust(15);

  // first 9x16 chars are junk
  for(int i = 0; i!=144; i++){
    in_stream.get(holderChar);
  }
  usleep(5000);

  /* Main loop of program */
  while(flying){
    int i = 0;
    while(in_stream.get(holderChar)){
      if(i == 15){
	controllerCommand = (int)holderChar;
	ardCommand = getArdCommand(controllerCommand);
	if(ardCommand == 'l'){
	  lights++;
	  tx.setLEDS(lights%2);
	}
	else if(ardCommand == '!'){
	  tx.halt();
	  break;
	}
	else tx.sendCommand(ardCommand, false);
	//tx.resetOrientation();  TEST IF THIS IS NEEDED, surely limits flight heavily    	
      }
      i++;
      if(i == 16){
	i = 0;	
      }
    }
  }
  return 0;
}

      

