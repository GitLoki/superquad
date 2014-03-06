#include <iostream>
#include <ncurses.h>
#include "../include/Tx/Tx.hpp"
#include "../include/Kinect/Kinect.hpp"
#include "../include/PhysicsModel/PhysicsModel.hpp"
#include "../include/Controller/boost_xbox_controller.hpp"
using namespace std;

int main (int argc, char** argv) {
  //Tx tx;
  //Kinect k;
  //XBoxControllerManager xbcm;
  PhysicsModel pm;

  cout << pm.init() << endl;

  bool end = false;
  char com;
  //int x,y,z,p;

  /* Start curses mode */
  initscr();
  clear();
  noecho();
  cbreak();
  refresh();

  cout << pm.startSimulation() << endl;

  com = getch();

  /* Main loop of program */
  while(!end) {
    //tx.sendCommand(com,true);
    pm.sendCommand(com);

    /*
    xbcm.GetInput(ch); -- yet to be written
    k.query(x,y,z,p); -- yet to test
    pm.rectify(x,y,z,p); -- yet to be written
    */

    usleep(29000); // Wait ~thirtieth second
    clrtoeol();
    
    com = getch();
    clear();
    printw("Char input: %d = %c \n", com, com);
    refresh();
    flushinp();

    if(com == '0') {
      end = true;
    }
  }

  endwin();

  return 0;
}
