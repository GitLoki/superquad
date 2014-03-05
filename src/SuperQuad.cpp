#include <iostream>
#include <ncurses.h>
#include "../include/Tx/Tx.hpp"
#include "../include/Kinect/Kinect.hpp"
#include "../include/PhysicsModel/PhysicsModel.hpp"
#include "../include/Controller/boost_xbox_controller.hpp"
using namespace std;

int main (int argc, char** argv) {
  Tx tx;
  Kinect k;
  XBoxControllerManager xbcm;
  PhysicsModel pm;

  bool finished = false;
  char ch;
  int x,y,z,p;

  initscr();

  while(!finished) {
    ch = getch();

    /*
    Want something like this:
    tx.SendCommand(ch,true);
    pm.SetRotation(ch);
    ?? xbcm.GetInput(ch); -- yet to be written
    k.query(x,y,z,p);
    pm.rectify(x,y,z,p); -- yet to be written
    */

    if(ch == '0') {
      finished = true;
    }
  }  

  return 0;
}
