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

    // initialise ratios as 1
    integrals.setValues(0.0,0.0,0.0);

    // initialise sensible starting location values 
    location.setValues(XCENTRE, YCENTRE, 400.0);

    // set destination
    destination.setValues(XCENTRE, YCENTRE, 900);

    // open logfile for logging, write heading
    //PIDlogfile.open ("logfile.txt");
    //PIDlogfile << "control values :: location\n";
    //PIDlogfile << "  [X, Y, Z]    :: [X, Y, Z]\n";
    
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
    // const double K = 400;
    // const double K2 = 300;
    // lower => more aggressive correction 

    // 150, 160, 1000 - last best config
    // 220, 220, 1200 :: 6k, 6k, 90k

    // Proportional parameters
    const double KP_x = 2000000;
    const double KP_y = 2000000;
    const double KP_z = 2000000;

    // Integral parameters
    const double KI_x = 10000000;
    const double KI_y = 10000000;
    const double KI_z = 10000000;

    // Differential parameters
    const double KD_x = 10000000;
    const double KD_y = 10000000;
    const double KD_z = 10000000;
    
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
    ratios.Y = 1 - (delta.Y / KP_y) - (integrals.Y / KI_y)
		- (partial_derivatives.Y / KD_y);
    ratios.Z = 1 + (delta.Z / KP_z); 
    // + integrals.Z * KI_z  - (partial_derivatives.Z * KD_z);;
      
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
  
    /*
    if (location.Z <= 40)
        control_vals[1] = STARTPOW;
    */

//    PIDlogfile << time_diff << "ms :: " 
//            << "[" << control_vals[2] << ", " 
//	      << control_vals[3] << ", " 
//	      << control_vals[0] << "] :: (" 
//	      << location.X << ", " 
//	      << location.Y << ", " 
//	      << location.Z << ")\n";

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

void PID::signalHandler( int signum )
{
  std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

  //pid->halt();
  //PIDlogfile.close();

  exit(signum);  

}

