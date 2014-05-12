#ifndef TX_HPP
#define TX_HPP

/*******************************/
/*     class: Tx     */
/*******************************/

#include <boost/asio.hpp>
#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

#define LEDS_ON 0x05
#define LEDS_OFF 0x06
#define FLIPS_ON 0x07
#define FLIPS_OFF 0x08

enum controlIndices {SIGNAL, THROTTLE, RUDDER, AILERON, ELEVATOR};

const int sigVal = 3;
const int aileronTrim = 127;
const int elevatorTrim = 137;
const int rudderTrim = 137;

class Tx {

public:
  /* constructor */
  Tx();

  /* destructor */
  ~Tx();

  /* sends a command to the Arduino; set verbose to true for debugging 
     information (function expects ncurses mode) */
  void sendCommand(char com, bool verbose);

  // sets flips or LEDS on or off.
  void setLEDS(bool active);
  void setFlips(bool active);


  /* send commands to alter and transmit a specific control value */
  void setThrottle(int _throttle);
  void setRudder(int _rudder);
  void setAileron(int _aileron);
  void setElevator(int _elevator);
  void setThrust(int _thrust);

  /* sets all values back to starting values (throttle = 0) and transmits them */
  void halt();

  /* sets rudder, aileron and elevator back to default values and transmits them */
  void resetOrientation();

  /* calls cancel on port - clears write buffer of serial port */
  void cancel();

  /* returns current command values */
  void getValues(int* _values);
  
  /* set all command values simultaneously */
  void setValues(int* _values);

private:
  /* pointer to the input-output service */


  /* pointer to the port we are writing data to */
  boost::asio::serial_port* port;

  /* input/output service */
  boost::asio::io_service ios;

  /* data_incoming, throttle/thrust, rudder/yaw, aileron/roll, elevator/pitch */
  boost::array<unsigned char,5> controls;
  
  /* sanitises the current command values and sends them to the transmitter */
  void sendValues(bool verbose = false);
};

#endif /* TX_HPP */
