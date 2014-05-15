#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/Tx/Tx.hpp"
using namespace std;

//MAKE SURE ANALOG LIGHT IS ON!!!!!

int main (int argc, char** argv) {

  Controller controller = Controller();
  
  controller.flight_loop();

  return 0;
}


      

