#include "../../include/PS2ControllerV2/V2.hpp"  

Controller::Controller(Tx* t){
  //intialize variables
  tx = t;
  flying = true;
  lights = 1; aileron = A_BASE; elevator = E_BASE; rudder = R_BASE;
  in_stream.open("/dev/input/js0");
  
  prev_thrust = 0;
  thrust = 70;

  //Check controller input found
  if(in_stream.fail()){
    std::cout << "Controller not found" << std::endl;
    exit(1);
  }
  //prepare for flight
  tx->setThrust(thrust);
  tx->setElevator(E_BASE);
  tx->setRudder(R_BASE);
  tx->setAileron(A_BASE);  

  //first 9x16 chars are junk - discard them
  for(int i = 0; i!=144; i++){ 
    in_stream.get(holderChar);
  }  
}

void Controller::set_lights(){
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
  if(controllerCommand[4] == 1){
    tx->sendCommand('+');
    thrust += 10;
    prev_thrust += 10; //NEED THIS?
  }
}

void Controller::thrust_down(){
  //R2 down
  if(controllerCommand[4] == 1){
    tx->sendCommand('-');
    thrust -= 10;
    prev_thrust -= 10; //NEED THIS?
  }
}

void Controller::set_thrust(){
  //power stick released
  if(controllerCommand[4] == 0){ thrust = prev_thrust;}
  //if power up
  else if(controllerCommand[5] < 0){
    //set prev thrust to thrust, so we can return to original thrust
    //when stick released
    prev_thrust = thrust;
    //limit thrust to 255
    thrust = std::min((thrust - ((controllerCommand[5]/128)*SPEED)), 255);
  }
  //otherwise power down
  else if(controllerCommand[5] > 0){
    prev_thrust = thrust;
    thrust = std::max((thrust-((controllerCommand[5]/127)*SPEED)), 0);
  }
  tx->setThrust(thrust);
}


void Controller::set_elevator(){
  //stop
  if(controllerCommand[5] == 0){ elevator = E_BASE;}
  //forwards
  //two's compliment binary: -128 to 127
  else if(controllerCommand[5] < 0){ elevator = E_BASE - ((controllerCommand[5]/128)*SPEED);}
  //backwards
  else if(controllerCommand[5] > 0){ elevator = E_BASE - ((controllerCommand[5]/127)*SPEED);}
  //send new Euler angle value to tx (arduino)
  tx->setElevator(elevator);
}

void Controller::set_aileron(){
  //Left joystick, x-axis
  //stop
  if(controllerCommand[5] == 0){ aileron = A_BASE;}
  //roll left
  else if(controllerCommand[5] < 0){ aileron = A_BASE - ((controllerCommand[5]/128)*SPEED);}
  //roll right
  else if(controllerCommand[5] > 0){ aileron = A_BASE - ((controllerCommand[5]/127)*SPEED);}
  tx->setAileron(aileron);
}

void Controller::set_rudder(){
  //Right joystick, x-axis
  //stop 
  if(controllerCommand[5] == 0){ rudder = R_BASE;}
  //turn right
  else if(controllerCommand[5] < 0){ rudder = R_BASE - ((controllerCommand[5]/128)*100);}
//turn left
  else if(controllerCommand[5] > 0){ rudder = R_BASE - ((controllerCommand[5]/127)*100);}
  tx->setRudder(rudder);
  }

void Controller::set_switch(){
  //Interpret controller instruction
  switch(controllerCommand[7]){
  case 0:
    set_aileron();
    break;
  case 1:
    set_elevator();
    break;
  case 2: 
    set_rudder();
    break;
  case 3:
    set_thrust();
    break;
  case 6:
    thrust_up();
    break;
  case 7:
    thrust_down();
    break;
  case 8:
    set_lights();
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
      set_switch();
    }
  }
}
