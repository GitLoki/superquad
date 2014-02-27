#include "../../include/SerialWriter/SerialWriter.hpp"

/* Constructor */
SerialWriter::SerialWriter() {

  boost::asio::io_service ios;

  SerialWriter::port = new  boost::asio::serial_port(ios);  

  port->set_option(boost::asio::serial_port_base::baud_rate(115200));
  port->set_option(boost::asio::serial_port_base::flow_control
		  (boost::asio::serial_port_base::flow_control::none));
  port->set_option(boost::asio::serial_port_base::parity
		  (boost::asio::serial_port_base::parity::none));
  port->set_option(boost::asio::serial_port_base::stop_bits
		  (boost::asio::serial_port_base::stop_bits::one));
  port->set_option(boost::asio::serial_port_base::character_size(8U));
  
  try  {
    port->open("/dev/ttyACM0");
  } catch(...) {
    std::cout << "Unable to open /dev/ttyACM0!" << std::endl;
    exit(1);
  }
  
  controls[0] = 3;
  controls[1] = 0;
  controls[2] = 127;
  controls[3] = 127;
  controls[4] = 127;

  port->write_some(boost::asio::buffer(controls));
}

/* Destructor */
SerialWriter::~SerialWriter() {
  port->close();
}

/* sends a command to the Arduino; set verbose to true for debugging 
   information*/
void SerialWriter::SendCommand(char com, bool verbose) {
  switch(com) {
    case 'd': // roll right
      ChangeValue(controls[3], 10);
      break;
    case 'a': // roll left
      ChangeValue(controls[3], -10);
      break;
    case 'w': // pitch forward
      ChangeValue(controls[4], 10);
      break;
    case 'x': // pitch backward
      ChangeValue(controls[4], -10);
      break;
    case 'q': // yaw right
      ChangeValue(controls[2], 10);
      break;
    case 'e': // yaw left
      ChangeValue(controls[2], -10);
      break;
    case '+': // thrust increase
      ChangeValue(controls[1], 10);
      break;
    case '-': // thrust decrease
      ChangeValue(controls[1], -10);
      break;
  }

  if(verbose) {
    int bytes = port->write_some(boost::asio::buffer(controls));

    printw("Char input: %d = %c \n", com, com);
    printw("Controls: \n");
    for (int i = 0 ; i < 5 ; i++) {
      printw("%d ",controls[i]);
    }
    printw("\n");
    printw("Bytes Written: %d \n",bytes);
  } else {
    SerialWriter::port->write_some(boost::asio::buffer(controls));
  }
}

/* helper function to sanitise data that will be written to Arduino 
   registers */
void SerialWriter::ChangeValue(unsigned char& x, int c) {
  if((int) x + c > 255) {
    x = 255;
  } else if((int) x + c < 0) {
    x = 0;
  } else {
    x +=c;
  }
}
