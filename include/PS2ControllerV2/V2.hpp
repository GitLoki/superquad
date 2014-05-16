#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../../include/Tx/Tx.hpp"

//Global Constants. Seperate variables to allow tuning
//base (neutral) value for elevator
const int E_BASE = 129;
//base (neutral) value for aileron
const int A_BASE = 127;
const int R_BASE = 127;
//multiplier for joystick values. used by aileron and elevator 
//rudder is less sensitive and so left as a percentage of 
//max value
const int SPEED = 40;

class Controller{
private:
  Tx* tx;
  //Next char in input stream
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
  int speed_increment;

  //Button 9
  void set_lights();
  //Button 10
  void abort();
  //Right joystick, vertical axis
  void set_thrust();
  //R1
  void thrust_up();
  //R2
  void thrust_down();
  //Left joystick, vertical axis
  void set_elevator();
  //Left joystick, horizontal axis
  void set_aileron();
  //Right joystick, horizontal axis
  void set_rudder();
  //interprets controller input
  void set_switch();
  //Stabilises flight, thrust aims to hover
  void resetOrientation();

public:
  Controller(Tx* t);
  void flight_loop();
  ~Controller(){ delete tx; }
};

#endif
