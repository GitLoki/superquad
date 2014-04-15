#ifndef ACCEL_CONTROL_HPP
#define ACCEL_CONTROL_HPP

class AccelerationControl {

private:
    double K = 5.0;
    int currentValue = 0;
    double setPoint = 0.0;
    int jerkLimit = 15;

public:
    AccelerationControl(double _K, double _currentValue, double _jerkLimit) :
	                K(_K), currentAcceleration(_currentAcceleration), jerkLimit(_jerkLimit) {}
    AccelerationControl() {}
    
    void changeSetPoint(double _setPoint);

    int query(double acceleration);
};

#endif
