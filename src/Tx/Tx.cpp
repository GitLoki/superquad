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
  for (int i = THROTTLE ; i < ELEVATOR ; i++)
    _values[i] = controls[i];
};

/* sends a command to the Arduino; set verbose to true for debugging 
   information*/
void Tx::sendCommand(char com, bool verbose) {
  switch(com) {
    case 'd': // roll right
      controls[AILERON] += 10;
      break;
    case 'a': // roll left
      controls[AILERON] -= 10;
      break;
    case 'w': // pitch forward
      controls[ELEVATOR] += 10;
      break;
    case 'x': // pitch backward
      controls[ELEVATOR] -= 10;
      break;
    case 'q': // yaw right
      controls[RUDDER] += 10;
      break;
    case 'e': // yaw left
      controls[RUDDER] -= 10;
      break;
    case '+': // thrust increase
      controls[THROTTLE] += 10;
      break;
    case '-': // thrust decrease
      controls[THROTTLE] -= 10;
      break;
  }

  if(verbose)
    printw("Char input: %d = %c \n", com, com);

  sendValues(verbose);
}

void Tx::sendValues(bool verbose) {
    // ensure all values are legal
    // N.B. avoid using three, for it is cursed. (reserved as control value)
    for (int i = THROTTLE ; i <= ELEVATOR ; i++) {
	if (controls[i] < 0)
	    controls[i] = 0;
	else if (controls[i] > 255)
	    controls[i] = 255;
	else if (controls[i] == sigVal)
	    controls[i]++;
    }
	
    if(verbose) {
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
