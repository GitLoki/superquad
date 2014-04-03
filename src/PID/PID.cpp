#include "../../include/PID/PID.hpp"

PID::PID(Kinect* _kinect, Tx* _tx) : kinect(_kinect), tx(_tx)
{
    tx->getValues(trim);
    trim[0] = STARTPOW;
    tx->getValues(control_vals);
    ratios.setValues(1.0,1.0,1.0);
    location.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, ZCENTRE);

    // open logfile for logging
    logfile.open ("logfile.txt");
    logfile << "control values :: location\n";
    logfile << "  [X, Y, Z]    :: [X, Y, Z]\n";

};

int PID::updateLocation() { 
    const double weight = 1.0;
    Location new_location;
    kinect->query(new_location.X, new_location.Y, new_location.Z);

    if (new_location.X == 0 && new_location.Y == 0 && new_location.Z == 0)
      return 0;

    location.X = weight * new_location.X + (1-weight) * location.X;
    location.Y = weight * new_location.Y + (1-weight) * location.Y;
    location.Z = weight * new_location.Z + (1-weight) * location.Z;
      
    return 1;
};

void PID::updateDestination(Location* _destination) {
    destination = *_destination;
}

int PID::updateRatios() {
    // const double K = 400;
    // const double K2 = 300;
    // lower => more aggressive correction 

    // 170, 160, 800 - last best config

    const double Kx = 150;
    const double Ky = 160;
    const double Kz = 800;

    
    if (updateLocation()) {
      /*
      ratios.X = pow(2, (destination.X - location.X) / K);
      ratios.Y = pow(2, (destination.Y - location.Y) / K);
      ratios.Z = pow(2, (destination.Z - location.Z) / K2);
      */
      ratios.X = (destination.X - location.X) / Kx + 1;
      ratios.Y = -(destination.Y - location.Y) / Ky + 1;
      ratios.Z = (destination.Z - location.Z) / Kz + 1;
      
      return 1;
    }
    else
      return 0;
}

int PID::goToDestination(Location& _currentLocation) {
  if (!updateRatios()) 
      for(int i = 0 ; i <= 20 ; i++) {
	usleep(100000);
	if (updateLocation()) {
	  break;
	}
	else {
	  if(i == 20) {
	    tx->halt();
	    return 0;
	  } 
	}
      }
    
  int controlIndices[] = {2, 3, 0};
  int index;
  
  control_vals[2] = ratios.X * trim[2];
  control_vals[3] = ratios.Y * trim[3];
  control_vals[0] = ratios.Z * STARTPOW;
	
  for (int i = 0 ; i < 3 ; i++) {
    index = controlIndices[i];
    if (control_vals[index] > 235)
      control_vals[index] = 235;
    if (control_vals[index] <= 20)
      control_vals[index] = 20;
  }
  
  if (location.Z <= 40)
    control_vals[1] = STARTPOW;
  
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

  control_vals[1] = 117;

  tx->setValues(control_vals);
  _currentLocation.setValues(location.X, location.Y, location.Z);
  
  return 1;
}


void signalHandler( int signum )
{
  std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

  //pid->tx->halt();
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

    /*
    std::cout << "Finished waiting. Initialising liftoff..." << std::endl;
    tx->setThrust(STARTPOW);

    while (currentLocation.Z < MINDEPTH)
        kinect->query(currentLocation.X, currentLocation.Y, currentLocation.Z);

    std::cout << "Hovering height reached. Handing control to PID loop." << std::endl;
    				   */ 
    while (pid->goToDestination(currentLocation)) {
      usleep(1000000 / FPS);
    }

    return 0;
}
