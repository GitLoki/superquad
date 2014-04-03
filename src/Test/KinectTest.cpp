#include "../include/Kinect/Kinect.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  Kinect k;
  k.save_frame();
  //k.save_video("test.avi",300);
  
  return 0;
}
