#include "../../include/PID/PID.hpp"

PID::PID(Kinect* _kinect, Tx* _tx, 
	 int _KP_lateral, int _KP_z,
         int _KI_lateral, int _KI_z, 
	 int _KD_lateral, int _KD_z,
         int _STARTPOW, int _MINDEPTH) : kinect(_kinect), tx(_tx)
{
    STARTPOW = _STARTPOW; // 140
    MINDEPTH = _MINDEPTH; // 800
    MAXTHROTTLE = 220;

    // lower parameters  => more aggressive correction 
    KP_lateral = _KP_lateral; // 6500
    KP_z = _KP_z; // 10000
    KI_lateral = _KI_lateral; // 40000
    KI_z = _KI_z; // 100000
    KD_lateral = _KD_lateral; // 40000
    KD_z = _KD_z; // 100000

    // Proportional parameters
    KP_x = KP_lateral;
    KP_y = KP_lateral;

    // Integral parameters
    KI_x = KI_lateral;
    KI_y = KI_lateral;

    // Differential parameters
    KD_x = KD_lateral;
    KD_y = KD_lateral;
    
    // Get starting (neutral) values out of the transmitter.
    tx->getValues(trim);
    tx->getValues(control_vals);

    // set neutral throttle to STARTPOW
    trim[0] = STARTPOW;

    // initialise ratios as 1
    ratios.setValues(1.0,1.0,1.0);

    // initialise ratios as 1
    integrals.setValues(0.0,0.0,0.0);

    // initialise sensible starting location values 
    location.setValues(XCENTRE, YCENTRE, 400.0);

    // set destination
    destination.setValues(XCENTRE, YCENTRE, 900);
    
    previous_time = clock();
	previous_delta.X = 0;
	previous_delta.Y = 0;
	previous_delta.Z = 0;
};

int PID::updateLocation() { 

    // we use a proportion of the old location and a proportion of the new location
    const double weight = 1.0;
    Location new_location;

    // get new location out of kinect
    if (kinect->query(new_location.X, new_location.Y, new_location.Z, NULL)) {
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

    current_time = clock();
    time_diff = (current_time - previous_time) / 1000; // milliseconds
    previous_time = current_time;

    delta.X = destination.X - location.X;
    delta.Y = destination.Y - location.Y;
    delta.Z = destination.Z - location.Z;

    integrals.X += delta.X;
    integrals.Y += delta.Y;
    integrals.Z += delta.Z;

    partial_derivatives.X = (delta.X - previous_delta.X) / time_diff;
    partial_derivatives.Y = (delta.Y - previous_delta.Y) / time_diff;
    partial_derivatives.Z = (delta.Z - previous_delta.Z) / time_diff;

    ratios.X = 1 - (delta.X / KP_x) - (integrals.X / KI_x) 
		- (partial_derivatives.X / KD_x);
    ratios.Y = 1 + (delta.Y / KP_y) + (integrals.Y / KI_y)
		- (partial_derivatives.Y / KD_y);
    ratios.Z = 1 + (delta.Z / KP_z) + (integrals.Z / KI_z)
		- (partial_derivatives.Z / KD_z);
      
    previous_delta.X = delta.X;
    previous_delta.Y = delta.Y;
    previous_delta.Z = delta.Z;

    std::cout << ratios.X << ", " << ratios.Y << ", " << ratios.Z << std::endl;

    return 1;
}

int PID::goToDestination(Location& _currentLocation) {
    if (!updateLocation()) {
        for(int i = 0 ; i <= 1000 ; i++) {
  	    usleep(1000);
	    if (updateLocation()) {
	        break;
	    }
	    else {
	        if(i == 1000) {
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
 

    if(control_vals[THROTTLE - 1] > MAXTHROTTLE) {
      std::cout << "Throttle: " << control_vals[THROTTLE] << std::endl; 
      control_vals[THROTTLE - 1] = MAXTHROTTLE;
    }

    std::cout << time_diff << "ms :: " 
	      << "[" << control_vals[AILERON - 1] << ", " 
	      << control_vals[ELEVATOR - 1] << ", " 
	      << control_vals[THROTTLE - 1] << "] :: (" 
	      << location.X << ", " 
	      << location.Y << ", " 
	      << location.Z << ")" << std::endl;

    tx->setValues(control_vals);
    _currentLocation.setValues(location.X, location.Y, location.Z);
  
    return 1;
}

void PID::halt() {
    tx->halt();
    return;
}

void PID::signalHandler( int signum )
{
  std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

  exit(signum);
}

