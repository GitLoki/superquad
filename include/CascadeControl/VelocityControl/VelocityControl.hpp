class VelocityControl{
 
private:
  double k;        // constant for correction
  int setPoint;    // target velocity
  int max;         // correction limit

public:
  VelocityControl(){k = 0.01; setPoint = 0; max = 0.05;}
  void changeSetPoint(double velocity_);
  double query(double velocity);
};
