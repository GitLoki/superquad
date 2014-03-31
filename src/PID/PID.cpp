#include "../../include/PID/PID.hpp"

PID::PID(Kinect* _kinect, Tx* _tx) : kinect(_kinect), tx(_tx)
{
    tx->getValues(control_vals);
    for (int i = 0 ; i < 3 ; i++) {
         location[i] = 0;
	ratios[i] = 1;
    }
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
    updateLocation();
    for (int i = 0 ; i < 3 ; i++)
        ratios[i] = pow(2, destination[i] - location[i]);
    if (location[2] == 0)
        ratios[2] = 1.2;
}

void PID::goToDestination() {
    update();
    int controlIndices[] = {2, 3, 0};
    for (int i = 0 ; i < 3 ; i++) {
        int index = controlIndices[i];
        control_vals[index] *= ratios[i];
	if (control_vals[index] > 250)
	    control_vals[index] = 250;
	if (control_vals[index] <= 0)
	    control_vals[index] = 5;
    }
    tx->setValues(control_vals);
}

int main() {
    Kinect* kinect = new Kinect;
    usleep(5000000);
    Tx* tx = new Tx;
    usleep(30000000);
    PID* pid = new PID(kinect, tx);
    while (true)
      pid->goToDestination();
    return 0;
}
