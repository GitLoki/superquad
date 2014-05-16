#include "../../include/PID/PID.hpp"

int main() {
      
    Location currentLocation, destination;
    currentLocation.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, 1400);

    std::cout << "Initialising Kinect..." << std::endl;
    Kinect* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    std::cout << "Initialising Transmitter..." << std::endl;
    Tx* tx = new Tx;
    std::cout << "Transmitter Initialised. Waiting for serial connection..." << std::endl;
    usleep(1000000 * COUNTDOWN);

    std::cout << "Waiting complete. Initiating automated control..." << std::endl;

    PID* pid = new PID(kinect, tx);

	// let the PID controller control the drone until the program is killed
    while (pid->goToDestination(currentLocation)) {
      //usleep(1000000 / FPS);
    }

    return 0;
}
