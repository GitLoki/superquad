#include "../../include/PID/PID.hpp"

PID::PID(Kinect* _kinect, Tx* _tx) : kinect(_kinect), tx(_tx)
{
    tx->getValues(control_vals);
    for (int i = 0 ; i < 3 ; i++) {
     	ratios[i] = 1;
    }
    location[0] = XCENTRE;
    location[1] = YCENTRE;
    location[2] = 0;
    destination[0] = XCENTRE;
    destination[1] = YCENTRE;
    destination[2] = ZCENTRE;
};

int PID::updateLocation() { 
    const double m_weighting = 0.3;
    double new_location[3];
    kinect->query(new_location[0], new_location[1], new_location[2]);
    for (int i = 0 ; i < 3 ; i++)
    location[i] = m_weighting * new_location[i] + (1-m_weighting) * location[i];
    return 0;
};

void PID::setDestination(double* _destination) {
    for (int i = 0 ; i < 3 ; i++)
        destination[i] = _destination[i];
    return;
};

void PID::update() {
    double K = 10000;
    updateLocation();
    for (int i = 0 ; i < 3 ; i++)
      ratios[i] = pow(2, (destination[i]/K) - (location[i]/K));
    if (location[2] == 0)
        ratios[2] = 1.2;
}

void PID::goToDestination() {
    update();
    int controlIndices[] = {4, 3, 1};
    for (int i = 0 ; i < 3 ; i++) {
        int index = controlIndices[i];
        control_vals[index] *= ratios[i];
	if (control_vals[index] > 250)
	    control_vals[index] = 250;
	if (control_vals[index] <= 0)
	    control_vals[index] = 5;
    }
    if (location[2] <= 40)
      control_vals[1] = STARTPOW;
    std::cout << "Sending control values: [" << control_vals[0] << ", " << control_vals[1] << ", " << control_vals[2] << ", " << control_vals[3] << ", " << control_vals[4] << "]" << std::endl;
    tx->setValues(control_vals);
    //tx->setThrust(control_vals[1]);
}

int main() {
    double X, Y, Z = 0;
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
    do 
      kinect->query(X, Y, Z);
    while (Z < 50);
    std::cout << "Hovering height reached. Handing control to PID loop." << std::endl;

    while (true) {
      pid->goToDestination();
      usleep(10000);
    }
    return 0;
}
