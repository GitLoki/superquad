 #ifndef CONTROL_HPP
#define CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"

class Control{

private:
    Location K;
    Location currentValue;
    Location setPoint;
    Location snapLimit;

public:
    Control(Location _K, Location _snapLimit) :
                    K(_K), snapLimit(_snapLimit) {}

    void changeSetPoint(Location _setPoint);

    virtual Location query(Location velocity) = 0;
};

#endif
