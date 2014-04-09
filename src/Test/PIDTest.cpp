#include "../../include/PID/PID.hpp"
#include "../../include/Kinect/Kinect.hpp"
#include "../../include/Tx/Tx.hpp"

void signalHandler( int signum )
{
  std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

  //pid->halt();
  logfile.close();

  exit(signum);  

}

int main() {
      
    Location currentLocation, destination;
    currentLocation.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, 1400);

    std::cout << "Initialising Kinect..." << std::endl;
    Sensor* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    std::cout << "Initialising Transmitter..." << std::endl;
    Actuator* tx = new Tx;
    std::cout << "Transmitter Initialised. Waiting for serial connection..." << std::endl;
    usleep(1000000 * COUNTDOWN);

    std::cout << "Waiting complete. Initiating automated control..." << std::endl;

    PID* pid = new PID(kinect, tx);

    // register signal SIGINT and signal handler  
    signal(SIGINT, signalHandler);  

    while (pid->goToDestination(currentLocation)) {
      //usleep(1000000 / FPS);
    }

    return 0;
}
