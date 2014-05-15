#ifndef CONTROL_HPP
#define CONTROL_HPP
#include "../datatypes.hpp"
#include "../include/config.hpp"

class Control{

protected:
    Location K;
    Location currentValue;
    Location setPoint;

public:
  Control(Location _K) :
    K(_K){}

  Control(Location _K, Location _startPoint) :
    K(_K), currentValue(_startPoint){}

  void changeSetPoint(Location _setPoint);

  virtual Location query(Location velocity) = 0;
};

#endif
