#include <iostream>
#include <cstdlib>
#include "../../include/PS2ControllerV2/V2.hpp"
#include "../../include/Tx/Tx.hpp"
using namespace std;

//This is the program to fly quadcopter with
//PS2 controller (V2 controls)

int main (int argc, char** argv) {

  //Create transmitter object
  Tx* tx = new Tx();

  //Create controller object
  Controller* controller = new Controller(tx);
  
  //Reads and interprets controlller instructions
  controller->flight_loop();

  return 0;
}


      

