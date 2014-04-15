#ifndef VELOCITY_CONTROL_HPP
#define VELOCITY_CONTROL_HPP

class VelocityControl{
 
private:
    double K = 2.0;              // constant for correction
    int setPoint = 0.0;          // target velocity
    int snapLimit = 5.0;         // correction limit

public:
    void changeSetPoint(double _velocity);
    double query(double velocity);
};

#endif
