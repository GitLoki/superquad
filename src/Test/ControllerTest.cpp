#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/PS2ControllerV2/V2.hpp"
#include "../../include/Tx/Tx.hpp"
using namespace std;

//MAKE SURE ANALOG LIGHT IS ON!!!!!

int main (int argc, char** argv) {

  Tx* tx = new Tx();

  Controller* controller = new Controller(tx);
  
  controller->flight_loop();

  return 0;
}


      

