#ifndef SENSOR_HPP
#define SENSOR_HPP

class Sensor {

public:
    virtual bool query(double& x, double& y, double& z) = 0;

};

#endif
