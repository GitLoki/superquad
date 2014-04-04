#include "../include/Kinect/Kinect.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  Kinect k;

  while(true){
    double x = 0.0, y = 0.0, z = 0.0;
    if (k.query(x,y,z)) {
	cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
    }
  }
  //k.save_frame();
  //k.save_video("test.avi",300);
  
  return 0;
}
