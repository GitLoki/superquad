#include "../include/Kinect/Kinect.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  Kinect k;

  // loop forever, printing out the the coordinates of any object the
  // kinect happens to be tracking - don't print anything if nothing
  // is being tracked
  while(true){
    double x = 0.0, y = 0.0, z = 0.0;
    if (k.query(x,y,z)) {
      cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
    }
  }
  
  return 0;
}
