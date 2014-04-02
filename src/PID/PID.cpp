#include "../../include/PID/PID.hpp"

PID::PID(Kinect* _kinect, Tx* _tx) : kinect(_kinect), tx(_tx)
{
    tx->getValues(control_vals);
    ratios.setValues(1.0,1.0,1.0);
    location.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, MINDEPTH);
};

int PID::updateLocation() { 
    const double weight = 1.0;
    Location new_location;
    kinect->query(new_location.X, new_location.Y, new_location.Z);

    location.X = weight * new_location.X + (1-weight) * location.X;
    location.Y = weight * new_location.Y + (1-weight) * location.Y;
    location.Z = weight * new_location.Z + (1-weight) * location.Z;
      
    return 0;
};

void PID::updateDestination(Location* _destination) {
    destination = *_destination;
}

void PID::updateRatios() {
    const double K = 4000;
    const double K2 = 800;
    
    updateLocation();

    ratios.X = pow(2, (destination.X / K) - (location.X / K));
    ratios.Y = pow(2, (destination.Y / K) - (location.Y / K));
    ratios.Z = pow(2, (destination.Z / K2) - (location.Z / K2));
}

void PID::goToDestination(Location& _currentLocation) {
    updateRatios();
    int controlIndices[] = {3, 4, 1};
    int index;

    control_vals[3] *= ratios.X;
    control_vals[4] *= ratios.Y;
    control_vals[1] *= ratios.Z;
	
    for (int i = 0 ; i < 3 ; i++) {
      index = controlIndices[i];
      if (control_vals[index] > 235)
	control_vals[index] = 235;
      if (control_vals[index] <= 20)
	control_vals[index] = 20;
    }

    if (location.Z <= 40)
      control_vals[1] = STARTPOW;

    std::cout << "[" << control_vals[3] << ", " 
	             << control_vals[4] << ", " 
                     << control_vals[1] << "] :: (" 
	             << location.X << ", " 
	             << location.Y << ", " 
	             << location.Z << ")" << std::endl;
    
    tx->setValues(control_vals);

    _currentLocation.setValues(location.X, location.Y, location.Z);
}

int main() {
    Location currentLocation, destination;
    currentLocation.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, MINDEPTH+100);

    std::cout << "Initialising Kinect..." << std::endl;
    Kinect* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    std::cout << "Initialising Transmitter..." << std::endl;
    Tx* tx = new Tx;
    std::cout << "Transmitter Initialised. Waiting for serial connection..." << std::endl;
    usleep(10000000);

    PID* pid = new PID(kinect, tx);
    std::cout << "Finished waiting. Initialising liftoff..." << std::endl;
    tx->setThrust(STARTPOW);

    while (currentLocation.Z < MINDEPTH)
        kinect->query(currentLocation.X, currentLocation.Y, currentLocation.Z);

    std::cout << "Hovering height reached. Handing control to PID loop." << std::endl;

    while (true) {
      pid->goToDestination(currentLocation);
      usleep(10000);
    }
    return 0;
}
