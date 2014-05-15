#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/Tx/Tx.hpp"

using namespace std;

//Global Constants
//base (neutral) value for elevator
const int E_BASE = 139;
//base (neutral) value for aileron
const int A_BASE = 127;
const int R_BASE = 137;
//multiplier for joystick values. used by aileron and elevator 
//rudder is less sensitive
const int SPEED = 25;

class Controller{
private:
  Tx* tx;
  char holderChar;
  int controllerCommand[8];
  int lights;
  bool flying;
  int aileron;
  int elevator;
  int rudder;
  ifstream in_stream;

  void do_lights();
  void abort();
  //TODO void resetOrientation;
  void thrust_up();
  void thrust_down();
  void do_elevator();
  void do_aileron();
  void do_rudder();
  void do_switch();

public:
  Controller(Tx* t);
  void flight_loop();
  ~Controller(){ delete tx; }
};

#endif
