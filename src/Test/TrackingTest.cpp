#include <iostream>
#include <fstream>
#include <ctime>
#include <ncurses.h>
#include "../include/Tx/Tx.hpp"
#include "../include/Kinect/Kinect.hpp"
#include "../include/PhysicsModel/PhysicsModel.hpp"
#include "../include/Controller/boost_xbox_controller.hpp"
using namespace std;

//CLOCKS_PER_SEC

int main(int argc, char** argv){

  Kinect k;
  Tx tx;

  /* Start curses mode */
  initscr();
  clear();
  noecho();
  cbreak();
  refresh();

  bool end = false;
  char com;
  double x,y,z,p;

  ofstream outf("recordings.txt");

  clock_t t;
  int new_time, old_time;
  int count = 0, counter = 0;
  float x_store = 0, y_store = 0, z_store = 0;

  com = getch();

  //tx.setThrust(0);

  t = clock(); old_time = int((t/CLOCKS_PER_SEC)*10);

  while(!end){

    k.query(x,y,z,p);

    t = clock();
    new_time = int((t/CLOCKS_PER_SEC)*1000/100);

    if(new_time > old_time && count){
      x_store /= count; y_store /= count; z_store /= count;
      /*
      outf << old_time << " : " << (int)x_store << " , "
	   << (int)y_store << " , " << (int)z_store
	   << "   (Readings: " << count << ")\n\n";
      */

      if(z_store > 850)	
	tx.sendCommand('-', false);
	tx.sendCommand('-', false);
      else if(z_store < 600)
	tx.sendCommand('+', false);
	tx.sendCommand('+', false);
      
      count = 0; x_store = x; y_store = y; z_store = z;

      old_time = new_time;


    } else {
      count++; x_store+=x; y_store+=y; z_store+=z;
    }

    /*
    clear();
    //printw("Char input: %d = %c \n", com, com);
    refresh();
    flushinp();

    if(com == '0') {
      end = true;
    }

    if(counter % 5 == 0) {
      tx.cancel();
    }
    
    counter++;
    */
    usleep(1000); // Wait ~thirtieth second
  }

  outf.close();

}
