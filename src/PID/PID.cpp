#include "../../include/PID/PID.hpp"

PID::PID(Sensor* _kinect, Actuator* _tx) : kinect(_kinect), tx(_tx)
{
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
    destination.setValues(XCENTRE, YCENTRE, 1000);

    // open logfile for logging, write heading
    logfile.open ("logfile.txt");
    logfile << "control values :: location\n";
    logfile << "  [X, Y, Z]    :: [X, Y, Z]\n";
    
    previous_time = clock();

};

int PID::updateLocation() { 

    // we use a proportion of the old location and a proportion of the new location
    const double weight = 1.0;
    Location new_location;

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

    // 220, 220, 1200 :: 6k, 6k, 90k

    // Proportional parameters
    const double KP_x = 165;
    const double KP_y = 180;
    const double KP_z = 1000;

    // Integral parameters
    const double KI_x = 250;
    const double KI_y = 250;
    const double KI_z = 4500;
    
    current_time = clock();
    time_diff = (current_time - previous_time) / 1000; // milliseconds
    previous_time = current_time;

    delta.X = (destination.X - location.X) / time_diff;
    delta.Y = (destination.Y - location.Y) / time_diff;
    delta.Z = (destination.Z - location.Z) / time_diff;

    integrals.X += delta.X;
    integrals.Y += delta.Y;
    integrals.Z += delta.Z;
    
    ratios.X = (destination.X - location.X) / KP_x + 1 + integrals.X / KI_x;
    ratios.Y = -(destination.Y - location.Y) / KP_y + 1 - integrals.Y / KI_y;
    ratios.Z = (destination.Z - location.Z) / KP_z + 1; // + integrals.Z / KI_z;
    
    //std::cout << ratios.X << ", " << ratios.Y << ", " << ratios.Z << std::endl;
      
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
  
    /*
    if (location.Z <= 40)
        control_vals[1] = STARTPOW;
    */

    logfile << time_diff << "ms :: " 
            << "[" << control_vals[2] << ", " 
	    << control_vals[3] << ", " 
	    << control_vals[0] << "] :: (" 
	    << location.X << ", " 
	    << location.Y << ", " 
	    << location.Z << ")\n";

    std::cout << time_diff << "ms :: " 
	      << "[" << control_vals[2] << ", " 
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

