#include "../../include/PS2ControllerV2/V2.hpp"  

Controller::Controller(Tx* t){
  //intialize variables
  tx = t;
  flying = true;
  lights = 1; aileron = A_BASE; elevator = E_BASE; rudder = R_BASE;
  in_stream.open("/dev/input/js0");
  
  //Check controller input found
  if(in_stream.fail()){
    cout << "Controller not found" << endl;
    exit(1);
  }
  //prepare for flight
  tx->setThrust(80);
  
  //first 9x16 chars are junk - discarded
  for(int i = 0; i!=144; i++){ 
    in_stream.get(holderChar);
  }  
}

void Controller::do_lights(){
  //Change boolean value in Tx
  //controllerCommand[4] == 0 means 'unpress' so do nothing
  if(controllerCommand[4] == 1){
    lights++;
    tx->setLEDS(lights%2);
  }
}

void Controller::abort(){
  tx->halt();
  //end flight loop
  flying = false;
}

void Controller::thrust_up(){
  //R1 up
  if(controllerCommand[4] == 1) tx->sendCommand('+');
}

void Controller::thrust_down(){
  //R2 down
  if(controllerCommand[4] == 1) tx->sendCommand('-');
}

void Controller::do_elevator(){
  //stop
  if(controllerCommand[4] == 0) elevator = E_BASE;
  //forwards
  //two's compliment binary: -128 to 127
  else if(controllerCommand[5] < 0){ 
    elevator = E_BASE - ((controllerCommand[5]/128)*SPEED);
    cout << ((controllerCommand[5]/128)*SPEED) << endl;}
  //backwards
  else elevator = E_BASE - ((controllerCommand[5]/127)*SPEED);
  //send new Euler angle value to tx (arduino)
  tx->setElevator(elevator);
}

void Controller::do_aileron(){
  //Left joystick, x-axis
  //stop
  if(controllerCommand[4] == 0) aileron = A_BASE;
  //roll left
  else if(controllerCommand[5] < 0) aileron = A_BASE - ((controllerCommand[5]/128)*SPEED);
  //roll right
  else aileron = A_BASE + ((controllerCommand[5]/128)*SPEED);
  tx->setAileron(aileron);
}

void Controller::do_rudder(){
  //Right joystick, x-axis
  //stop 
  if(controllerCommand[4] == 0) rudder = R_BASE;
  //turn right
  else if(controllerCommand[5] > 0) rudder = R_BASE + ((controllerCommand[5]/127)*100);
  //turn left
  else rudder = R_BASE - ((controllerCommand[5]/128)*100);
  tx->setRudder(rudder);
}

void Controller::do_switch(){
  //Interpret controller instruction
  switch(controllerCommand[7]){
  case 0:
    do_aileron();
    break;
  case 1:
    do_elevator();
    break;
  case 2: 
    do_rudder();
    break;
  case 6:
    thrust_up();
    break;
  case 7:
    thrust_down();
    break;
  case 8:
    do_lights();
    break;
  case 9:
    abort();
    break; 
  }  
}

void Controller::flight_loop(){
  while(flying){
    if(!in_stream.eof()){
      //get next controller instruction
      for(int i = 0 ; i < 8 ; i++){
	in_stream.get(holderChar);
	controllerCommand[i] = int(holderChar);
      }
      //interpret and execute instruction
      do_switch();
    }
  }
}
