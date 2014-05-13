#include "../../include/Tx/Tx.hpp"

/* Constructor */
Tx::Tx() {
  port = new boost::asio::serial_port(ios);  

  try  {
    port->open("/dev/ttyACM0");
  } catch(...) {
    std::cout << "Unable to open /dev/ttyACM0!" << std::endl;
    exit(1);
  }

  usleep(2000000);

  port->set_option(boost::asio::serial_port_base::baud_rate(115200));
  port->set_option(boost::asio::serial_port_base::flow_control
		  (boost::asio::serial_port_base::flow_control::none));
  port->set_option(boost::asio::serial_port_base::parity
		  (boost::asio::serial_port_base::parity::none));
  port->set_option(boost::asio::serial_port_base::stop_bits
		  (boost::asio::serial_port_base::stop_bits::one));
  port->set_option(boost::asio::serial_port_base::character_size(8U));
  
  // initialise default control values and send them to the Tx
  halt();
  sendValues(false);
}

/* Destructor */
Tx::~Tx() {
  port->close();
}

void Tx::resetOrientation() {
  controls[RUDDER] = rudderTrim;
  controls[AILERON] = aileronTrim;
  controls[ELEVATOR] = elevatorTrim;
  sendValues();
}

void Tx::setThrust(int _thrust) {
  controls[THROTTLE] = _thrust;
  sendValues();
}

void Tx::setRudder(int _yaw) {
  controls[RUDDER] = _yaw;
  sendValues();
}

void Tx::setAileron(int _roll) {
  controls[AILERON] = _roll;
  sendValues();
}

void Tx::setElevator(int _pitch) {
  controls[ELEVATOR] = _pitch;
  sendValues();
}

void Tx::halt() {
  controls[SIGNAL] = sigVal;
  controls[THROTTLE] = 0;
  controls[RUDDER] = rudderTrim;
  controls[AILERON] = aileronTrim;
  controls[ELEVATOR] = elevatorTrim;
    
  sendValues();
}

void Tx::getValues(int* _values) {
  for (int i = THROTTLE ; i <= ELEVATOR ; i++)
    _values[i-1] = controls[i];
};

void Tx::getValues(Location* values) {
  values->X = controls[AILERON];
  values->Y = controls[ELEVATOR];
  values->Z = controls[THROTTLE];
}

void Tx::setValues(int* _values) {
  for (int i = THROTTLE ; i <= ELEVATOR ; i++)
      controls[i] = _values[i-1];
  sendValues();
};

/* sends a command to the Arduino; set segfault to true for debugging 
   information*/
void Tx::sendCommand(char com, bool segfault = false) {
  switch(com) {
  case 'd': // roll right
    controls[AILERON] = controls[AILERON] <= 245 ? controls[AILERON] + 5 : 255; 
    break;
  case 'a': // roll left
    controls[AILERON] = controls[AILERON] >= 10 ? controls[AILERON] - 5 : 0; 
    break;
  case 'w': // pitch forward
    controls[ELEVATOR] += 5;
    break;
  case 'x': // pitch backward
    controls[ELEVATOR] -= 5;
    break;
  case 'q': // yaw right
    controls[RUDDER] += 5;
    break;
  case 'e': // yaw left
    controls[RUDDER] -= 5;
    break;
  case '+': // thrust increase
    controls[THROTTLE] = controls[THROTTLE] <= 245 ? controls[THROTTLE] + 10 : 255; 
    break;
  case '-': // thrust decrease
    controls[THROTTLE] = controls[THROTTLE] >= 10 ? controls[THROTTLE] - 10 : 0; 
    break;
  case ' ':
    controls[RUDDER] = rudderTrim;
    controls[ELEVATOR] = elevatorTrim;
    controls[AILERON] = aileronTrim;
    break;
  }

  if(segfault)
    printw("Char input: %d = %c \n", com, com);

  sendValues(segfault);
}

void Tx::setValues(Location values) {
    controls[AILERON] = values.X;
    controls[ELEVATOR] = values.Y;
    controls[THROTTLE] = values.Z;
    sendValues(false);
}

void Tx::setLEDS(bool active) {
    uint8_t settings[5];
    settings[0] = 4;
    settings[1] = (active ? LEDS_ON : LEDS_OFF);
    settings[2] = 0;
    settings[3] = 0;
    settings[4] = 0;

    port->write_some(boost::asio::buffer(settings));
} 

void Tx::setFlips(bool active) {
    uint8_t settings[5];
    settings[0] = 4;
    settings[1] = (active ? FLIPS_ON : FLIPS_OFF);
    settings[2] = 0;
    settings[3] = 0;
    settings[4] = 0;

    port->write_some(boost::asio::buffer(settings));
} 

void Tx::sendValues(bool segfault) {
    // ensure all values are legal
    // N.B. avoid using three, for it is cursed. (reserved as control value)
    // Deprecated - santising input inside sendCommand function instead
  /*
    for (int i = THROTTLE ; i <= ELEVATOR ; i++) {
	if (controls[i] < 0)
	    controls[i] = 0;
	else if (controls[i] > 255)
	    controls[i] = 255;
	else if (controls[i] == sigVal)
	    controls[i]++;
    }
  */
	// avoid using packet header values as control values
  for ( int i = 1 ; i < 5 ; i++ ) {
      if (controls[i] == CONTROL_PACKET || controls[i] == SETTING_PACKET) {
          controls[i] = SETTING_PACKET + 1;
      }
  }

    if(segfault) {
	int bytes = port->write_some(boost::asio::buffer(controls));

	printw("Controls: \n");
	for (int i = 0 ; i < 5 ; i++) {
	    printw("%d ",controls[i]);
	}
	printw("\n");
	printw("Bytes Written: %d \n",bytes);
    } else {
	port->write_some(boost::asio::buffer(controls));
    }
}   

void Tx::cancel() {
  port->cancel();
}
