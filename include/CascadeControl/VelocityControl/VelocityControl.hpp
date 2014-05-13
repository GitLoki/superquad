class VelocityControl{
 
private:
  double k = 2;              // constant for correction
  int setPoint = 0;          // target velocity
  int snapLimit = 5;         // correction limit

public:
  void changeSetPoint(double _velocity);
  double query(double velocity);
};
