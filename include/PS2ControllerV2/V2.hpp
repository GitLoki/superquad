#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/Tx/Tx.hpp"

//Global Constants
//base (neutral) value for elevator
const int E_BASE = 147;
//base (neutral) value for aileron
const int A_BASE = 127;
const int R_BASE = 137;
//multiplier for joystick values. used by aileron and elevator 
//rudder is less sensitive
const int SPEED = 40;

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
  std::ifstream in_stream;
  int thrust;
  //when thrust joystick released we can return to previous thrust
  int prev_thrust;

  void set_lights();
  void abort();
  //TODO void resetOrientation;
  void set_thrust();
  void thrust_up();
  void thrust_down();
  void set_elevator();
  void set_aileron();
  void set_rudder();
  void set_switch();
  void resetOrientation();

public:
  Controller(Tx* t);
  void flight_loop();
  ~Controller(){ delete tx; }
};

#endif
