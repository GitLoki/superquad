#include "../../../include/CascadeControl/VelocityControl/VelocityControl.hpp"
#include <math.h> 

void VelocityControl::changeSetPoint(int _velocity){
  setPoint = velocity_;
}

double VelocityControl::query(double velocity){
  double acceleration = floor(k*(velocity - setPoint));
  if(acceleration > snapLimit) acceleration = snapLimit;
  else if(acceleration < 0 - snapLimit) acceleration = 0 - snapLimit;
  return acceleration;
}
