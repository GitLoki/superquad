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
  default: return ' ';
  }
}

int main (int argc, char** argv) {

  Tx tx;
  char holderChar;
  char ardCommand;
  

  ifstream in_stream;
  in_stream.open("/dev/input/js0");

  tx.setLEDS(false);

  tx.setThrust(15);

  // first 9x16 chars are junk
  for(int i = 0; i!=144; i++){
    in_stream.get(holderChar);
  }
  usleep(5000);

  /* Main loop of program */
  // for(int i=0; i!=5; i++){
  //   tx.setThrust(i*15);
  //   usleep(1000000);
  // }

  // while(true){
  //   cout<< "Sending d " << endl;
  //   tx.sendCommand('d', false);
  //   usleep(10000);
  //   tx.sendCommand('d', false);
  //   usleep(10000);
  //   tx.sendCommand('d', false);
  //   usleep(1000000);
  // }

  int i = 0;
  while(true){
    if(in_stream.peek() == EOF){ continue;}

    in_stream.get(holderChar);

    if(i == 15){
      ardCommand = getArdCommand( (int) holderChar);
      // returns '?' if illegal key
      if(ardCommand != '?') {
  	tx.sendCommand(ardCommand, false);     
  	cout << (int) holderChar;
      }
    }
    i++;
    if(i == 16){
      i = 0;
    }
    usleep(5000);
  }

  return 0;
}

      
//tx.resetOrientation();      	y
