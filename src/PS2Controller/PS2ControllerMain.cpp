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
  default: return '?';
  }
}

int main (int argc, char** argv) {

  Tx tx;
  char holderChar;
  int controllerCommand;
  char ardCommand;
  

  ifstream in_stream;
  in_stream.open("/dev/input/js0");
  tx.setThrust(80);

  // first 9x16 chars are junk
  for(int i = 0; i!=144; i++){
    in_stream.get(holderChar);
  }

  /* Main loop of program */
  while(true){
    int i = 0;
    while(in_stream.get(holderChar)){
      if(i == 15){
	controllerCommand = (int)holderChar;
	ardCommand = getArdCommand(controllerCommand);
	// returns '?' if illegal key
	if(ardCommand != '?') tx.sendCommand(ardCommand, true); 
	tx.resetOrientation();      	
      }
      i++;
      if(i == 16){
	i = 0;
      }
    }
  }
  return 0;
}

      

