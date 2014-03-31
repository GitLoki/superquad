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

  // Start curses mode 
  initscr();
  clear();
  noecho();
  cbreak();
  refresh();

  bool end = false;
  char com;
  double x,y,z,p;

  //ofstream outf("recordings.txt");

  clock_t t;
  int new_time, old_time;
  int count = 0, counter = 0;
  float x_store = 0, y_store = 0, z_store = 0;
  bool readings = false;

  com = getch();

  //tx.setThrust(0);

  t = clock(); old_time = int((t/CLOCKS_PER_SEC)*10);

  int i = 0;

  while(true){

    k.query(x,y,z);

    if(x+y+z == 0)
      readings = false;
    else
      readings = true;

    t = clock();
    new_time = int((t/CLOCKS_PER_SEC)*1000/100);

    if(new_time > old_time && count){
      x_store /= count; y_store /= count; z_store /= count;
      
      printw("Readings: %d %d %d \n", x_store, y_store, z_store);

      //outf << old_time << " : " << (int)x_store << " , "
      //   << (int)y_store << " , " << (int)z_store
      //   << "   (Readings: " << count << ")\n\n";
      //


      if(readings && x_store > 350) {
	tx.sendCommand('a', false);
	cout << "LEFT\n\n" << endl;
      } else if (readings && x_store < 150) {
	tx.sendCommand('d', false);
	cout << "RIGHT\n\n" << endl;
      }

      if(readings && y_store > 150) {
	tx.sendCommand('x', false);
	cout << "BACKWARD\n\n" << endl;
      } else if (readings && y_store < 250) {
	tx.sendCommand('w', false);
	cout << "FORWARD\n\n" << endl;
      }
      
      if(readings && z_store > 900) {
	tx.sendCommand('-', false);
	tx.sendCommand('-', false);
	cout << "UP\n\n" << endl;
      }
      else if(!readings || z_store < 700) {
	tx.sendCommand('+', false);
	tx.sendCommand('+', false);
	cout << "DOWN\n\n" << endl;
      }

      count = 0; x_store = x; y_store = y; z_store = z;

      old_time = new_time;

    } else {
      count++; x_store+=x; y_store+=y; z_store+=z;
    }

    
    //clear();
    //printw("Char input: %d = %c \n", com, com);
    //refresh();
    //flushinp();

    //if(com == '0') {
    //  end = true;
    //}

    //if(counter % 5 == 0) {
    //  tx.cancel();
    //}
    
    //counter++;
  
    usleep(1000); // Wait ~thirtieth second
  }

  //outf.close();

  

}

