#include <iostream>
#include <cstdlib>
#include <fstream>
#include "stdlib.h"
#include "../../include/Tx/Tx.hpp"

using namespace std;

class Controller{
private:
  Tx tx;
  char holderChar;
  int controllerCommand[8];
  int lights = 1;
  bool flying = true;
  int aileron = 127;
  int elevator = 137;
  int rudder = 137;  
  ifstream in_stream;

  //base (neutral) value for elevator and rudder
  const int ER_BASE = 137;
  //base (neutral) value for aileron
  const int A_BASE = 127;
  //multiplier for joystick values. used by aileron and elevator 
  //rudder is less sensitive
  const int SPEED = 25;

  void lights();
  void abort();
  //TODO void resetOrientation;
  void thrust_up();
  void thrust_down();
  void elevator();
  void aileron();
  void rudder();
  void switch();
  void flight_loop();

public:
  Controller();
};
