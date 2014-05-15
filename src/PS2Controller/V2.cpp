#include "V2.hpp"

Controller::Controller(){
  //intialize variables
  tx = Tx();
  flying = true;
  lights = 1; aileron = 127; elevator = 137; rudder = 137;
  in_stream.open("/dev/input/js0");
  
  //Check controller input found
  if(in_stream.fail()){
    cout << "Controller not found" << endl;
    exit(1);
  }
  //prepare for flight
  tx.setThrust(60);
  
  //first 9x16 chars are junk - discarded
  for(int i = 0; i!=144; i++){ 
    in_stream.get(holderChar);
  }  
}
  
Controller::void lights(){
  //Change boolean value in Tx
  lights++;
  tx.setLEDS(lights%2);
}

Controller::void abort(){
  tx.halt();
  //end flight loop
  flying = false;
}

Controller::void thrust_up(){
  //R1 up
  if(controllerCommand[4] == 1) tx.sendCommand('+');
}

Controller::void thrust_down(){
  //R2 down
  if(controllerCommand[4] == 1) tx.sendCommand('-');
}

Controller::void elevator(){
  //stop
  if(controllerCommand[4] == 0) elevator = ER_BASE;
  //forwards
  //two's compliment binary: -128 to 127
  else if(controllerCommand[5] < 0) elevator = ER_BASE - ((controllerCommand[5]/128)*SPEED);
  //backwards
  else elevator = ER_BASE + ((controllerCommand[5]/127)*SPEED);
  //send new Euler angle value to tx (arduino)
  tx.setElevator(elevator);
}

Controller::void aileron(){
  //Left joystick, x-axis
  //stop
  if(controllerCommand[4] == 0) aileron = A_BASE;
  //roll left
  else if(controllerCommand[5] < 0) aileron = A_BASE - ((controllerCommand[5]/128)*SPEED);
  //roll right
  else aileron = A_BASE + ((controllerCommand[5]/128)*SPEED);
  tx.setAileron(aileron);
}

Controller::void rudder(){
  //Right joystick, x-axis
  //stop 
  if(controllerCommand[4] == 0) rudder = ER_BASE;
  //turn right
  else if(controllerCommand[5] > 0) rudder = ER_BASE + ((controllerCommand[5]/127)*100);
  //turn left
  else rudder = ER_BASE - ((controllerCommand[5]/128)*100);
  tx.setRudder(rudder);
}

Controller::void switch(){
  //Interpret controller instruction
  switch(controllerCommand[7]){
  case 0:
    aileron();
    break;
  case 1:
    elevator();
    break;
  case 2: 
    rudder();
    break;
  case 6:
    thrust_up();
    break;
  case 7:
    thrust_down();
    break;
  case 8:
    lights();
    break;
  case 9:
    abort();
    break; 
  }  
 }

Controller::void flight_loop(){
  while(flying){
    if(!in_stream.eof()){
      //get next controller instructions
      for(int i - 0; i < 8; i++){
	in_stream.get(holderChar);
	controllerCommand[i] = int(holderChar);
      }
      //interpret and execute instruction
      switch();
    }
  }
}
