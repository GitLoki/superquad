#ifndef SERIALWRITER_HPP
#define SERIALWRITER_HPP

/*******************************/
/*     class: SerialWriter     */
/*******************************/

#include <boost/asio.hpp>
#include <ncurses.h>
#include <iostream>
#include <cstdlib>

class SerialWriter {

public:
  /* constructor */
  SerialWriter();

  /* destructor */
  ~SerialWriter();

  /* sends a command to the Arduino; set verbose to true for debugging 
     information (function expects ncurses mode) */
  void SendCommand(char com, bool verbose);

private:
  /* pointer to the input-output service */


  /* pointer to the port we are writing data to */
  boost::asio::serial_port* port;

  /* data_incoming, throttle/thrust, rudder/yaw, aileron/roll, elevator/pitch */
  boost::array<unsigned char,5> controls;

  /* helper function to sanitise data that will be written to Arduino 
     registers */
  void ChangeValue(unsigned char& x, int c);
};

#endif /* SERIALWRITER_HPP */
