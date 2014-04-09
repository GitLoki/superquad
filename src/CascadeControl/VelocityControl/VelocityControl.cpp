#include <VelocityControl.hpp>
#include <math.h> 

void VelocityControl::changeSetPoint(int velocity_){
  setPoint = velocity_;
}

double VelocityControl::query(double velocity){
  double acceleration = floor(k*(velocity - setPoint));
  if(acceleration > max) acceleration = max;
  else if(acceleration < 0 - max) acceleration = 0 - max;
  return acceleration;
}
