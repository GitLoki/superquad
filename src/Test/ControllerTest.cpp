#include <iostream>
#include <cstdlib>
#include "../../include/PS2ControllerV2/V2.hpp"
#include "../../include/Tx/Tx.hpp"
#include "../../include/PhysicsModel/PhysicsModel.hpp"
using namespace std;

//MAKE SURE ANALOG LIGHT IS ON!!!!!

int main (int argc, char** argv) {

  Tx* tx = new Tx();

  PhysicsModel* p = new PhysicsModel();

  Controller* controller = new Controller(tx, p);
  
  controller->flight_loop();

  return 0;
}


      

