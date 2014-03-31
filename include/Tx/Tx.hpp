#ifndef TX_HPP
CvVideoWriter* videoWriter = cvCreateVideoWriter(outputFilename, CV_FOURCC('D','I','V','3'), FPS, combinedSize, TRUE);
IplImage *frameOut = cvCreateImage(combinedSize, 8, 3);		// Create an empty RGB image for storing the combined frame.
IplImage *frame1, *frame2;

// Process both video streams while atleast one is still running (AVI or MPG file or camera stream).
frame1 = (IplImage*)1;	// Enter the loop.
frame2 = (IplImage*)1;	// Enter the loop.
while (frame1 || frame2) {
	// Get the next video frames.
	frame1 = cvQueryFrame( capture1 );
	frame2 = cvQueryFrame( capture2 );
	if (frame1 || frame2) {
		// Combine the 2 image frames into 1 big frame.	
		frameOut = combineImages(frame1, frame2);
		// Store the combined video frame into the new video file.
		cvWriteFrame(videoWriter, frameOut);
	}
}
CvVideoWriter* videoWriter = cvCreateVideoWriter(outputFilename, CV_FOURCC('D','I','V','3'), FPS, combinedSize, TRUE);
IplImage *frameOut = cvCreateImage(combinedSize, 8, 3);		// Create an empty RGB image for storing the combined frame.
IplImage *frame1, *frame2;

// Process both video streams while atleast one is still running (AVI or MPG file or camera stream).
frame1 = (IplImage*)1;	// Enter the loop.
frame2 = (IplImage*)1;	// Enter the loop.
while (frame1 || frame2) {
	// Get the next video frames.
	frame1 = cvQueryFrame( capture1 );
	frame2 = cvQueryFrame( capture2 );
	if (frame1 || frame2) {
		// Combine the 2 image frames into 1 big frame.	
		frameOut = combineImages(frame1, frame2);
		// Store the combined video frame into the new video file.
		cvWriteFrame(videoWriter, frameOut);
	}
}
CvVideoWriter* videoWriter = cvCreateVideoWriter(outputFilename, CV_FOURCC('D','I','V','3'), FPS, combinedSize, TRUE);
IplImage *frameOut = cvCreateImage(combinedSize, 8, 3);		// Create an empty RGB image for storing the combined frame.
IplImage *frame1, *frame2;

// Process both video streams while atleast one is still running (AVI or MPG file or camera stream).
frame1 = (IplImage*)1;	// Enter the loop.
frame2 = (IplImage*)1;	// Enter the loop.
while (frame1 || frame2) {
	// Get the next video frames.
	frame1 = cvQueryFrame( capture1 );
	frame2 = cvQueryFrame( capture2 );
	if (frame1 || frame2) {
		// Combine the 2 image frames into 1 big frame.	
		frameOut = combineImages(frame1, frame2);
		// Store the combined video frame into the new video file.
		cvWriteFrame(videoWriter, frameOut);
	}
}
#define TX_HPP

/*******************************/
/*     class: Tx     */
/*******************************/

#include <boost/asio.hpp>
#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

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
  void sendValues(bool verbose);
};

#endif /* TX_HPP */
