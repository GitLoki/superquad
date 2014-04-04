#include "../../include/PID/PID.hpp"

PID::PID(Kinect* _kinect, Tx* _tx) : kinect(_kinect), tx(_tx)
{
    // Get starting (neutral) values out of the transmitter.
    tx->getValues(trim);
    tx->getValues(control_vals);

    // set neutral throttle to STARTPOW
    trim[0] = STARTPOW;

    // initialise ratios as 1
    ratios.setValues(1.0,1.0,1.0);

    // initialise sensible starting location values 
    location.setValues(XCENTRE, YCENTRE, 400.0);

    // set destination
    destination.setValues(XCENTRE, YCENTRE, 750);

    // open logfile for logging, write heading
    logfile.open ("logfile.txt");
    logfile << "control values :: location\n";
    logfile << "  [X, Y, Z]    :: [X, Y, Z]\n";

};

int PID::updateLocation() { 

    // we use a proportion of the old location and a proportion of the new location
    const double weight = 1.0;
    Location new_location;
    kinect->query(new_location.X, new_location.Y, new_location.Z);

    // get new location out of kinect
    if (kinect->query(new_location.X, new_location.Y, new_location.Z)) {
        // else, update the current location according to new_locatikn and weight
        location.X = weight * new_location.X + (1-weight) * location.X;
        location.Y = weight * new_location.Y + (1-weight) * location.Y;
        location.Z = weight * new_location.Z + (1-weight) * location.Z;
      
	return 1;
    }
    else
        return 0;
};

void PID::updateDestination(Location* _destination) {
    // simply set destination to the provided location
    destination = *_destination;
}

int PID::updateRatios() {
    // const double K = 400;
    // const double K2 = 300;
    // lower => more aggressive correction 

    // 150, 160, 1000 - last best config

    // Proportional parameters
    const double KP_x = 150;
    const double KP_y = 160;
    const double KP_z = 1000;

    // Integral parameters
    const double KI_x = 10000;
    const double KI_y = 10000;
    const double KI_z = 10000;
    
    if (updateLocation()) {
      integrals.X += (destination.X - location.X);
      integrals.Y += (destination.Y - location.Y);
      integrals.Z += (destination.Z - location.Z);

      ratios.X = (destination.X - location.X) / KP_x + 1 + integrals.X / KI_x;
      ratios.Y = -(destination.Y - location.Y) / KP_y + 1 - integrals.Y / KI_y;
      ratios.Z = (destination.Z - location.Z) / KP_z + 1 + integrals.Z / KI_z;
      
      return 1;
    }
    else
      return 0;
}

int PID::goToDestination(Location& _currentLocation) {
    if (!updateLocation()) {
        for(int i = 0 ; i <= 2000 ; i++) {
  	    usleep(10000);
	    if (updateLocation()) {
	        break;
	    }
	    else {
	        if(i == 2000) {
		    tx->halt();
		    return 0;
		} 
	    }
	}
    }
    
    updateRatios();
    
    int controlIndices[] = {2, 3, 0};
    int index;
  
    control_vals[2] = ratios.X * trim[2];
    control_vals[3] = ratios.Y * trim[3];
    control_vals[0] = ratios.Z * STARTPOW;
	
    for (int i = 0 ; i < 3 ; i++) {
        index = controlIndices[i];
	if (control_vals[index] > 250)
	    control_vals[index] = 250;
	if (control_vals[index] <= 5)
	    control_vals[index] = 5;
    }
  
    /*
    if (location.Z <= 40)
        control_vals[1] = STARTPOW;
    */

    logfile << "[" << control_vals[2] << ", " 
	    << control_vals[3] << ", " 
	    << control_vals[0] << "] :: (" 
	    << location.X << ", " 
	    << location.Y << ", " 
	    << location.Z << ")\n";

    std::cout << "[" << control_vals[2] << ", " 
	      << control_vals[3] << ", " 
	      << control_vals[0] << "] :: (" 
	      << location.X << ", " 
	      << location.Y << ", " 
	      << location.Z << ")" << std::endl;

    // safeguard against rudder being changed. Also, adjust the rudder trim
    control_vals[1] = 117; 

    tx->setValues(control_vals);
    _currentLocation.setValues(location.X, location.Y, location.Z);
  
    return 1;
}

void PID::halt() {
    tx->halt();
    return;
}

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
    Kinect* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    std::cout << "Initialising Transmitter..." << std::endl;
    Tx* tx = new Tx;
    std::cout << "Transmitter Initialised. Waiting for serial connection..." << std::endl;
    usleep(1000000 * COUNTDOWN);

    PID* pid = new PID(kinect, tx);

    // register signal SIGINT and signal handler  
    signal(SIGINT, signalHandler);  

    while (pid->goToDestination(currentLocation)) {
      //usleep(1000000 / FPS);
    }

    return 0;
}
