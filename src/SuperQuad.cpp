#include <iostream>
#include <ncurses.h>
#include "../include/Tx/Tx.hpp"
#include "../include/Kinect/Kinect.hpp"
#include "../include/PhysicsModel/PhysicsModel.hpp"
#include "../include/Controller/boost_xbox_controller.hpp"
#include <pthread.h>
#include <assert.h>

//QT includes
#include "GUI/gui_interface.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>
#include "GUI/pollthread.h"
#include "../include/Monitor/Monitor.hpp"

using namespace std;

struct argstruct {
    int argcs;
    char **argvs;
    Monitor *m;
};

void *contfun(void *argument);

void *guifun(void *argument);

int main (int argc, char** argv) {

    int ret_code;
    Monitor *mon = new Monitor;
    argstruct *arguments = new argstruct;
    
    arguments->argcs = argc;
    arguments->argvs = argv;
    arguments->m = mon;

    pthread_t guithread, controlthread;

    //have both threads call their respective functions
    ret_code = pthread_create(&guithread,NULL,guifun, (void*) arguments);
    assert(ret_code == 0);

    ret_code = pthread_create(&controlthread,NULL,contfun, (void*) arguments);
    assert(ret_code == 0);

    //wait for both threads to finish
    ret_code = pthread_join(guithread, NULL);
    assert(ret_code == 0);
    
    ret_code = pthread_join(controlthread, NULL);
    assert(ret_code == 0);
    
    return 0;

}

void *contfun(void *argument){

    argstruct *args = (argstruct*) argument;

    while(true);

    return NULL;
}

void *guifun(void *argument){

    argstruct *args = (argstruct*) argument;

    qRegisterMetaType<std::vector<float> >("std::vector<float>");

    QApplication quadcop(args->argcs, args->argvs);
    GUI_interface w(args->m);

    w.show();
    
    quadcop.exec();

    return NULL;
}








// int main (int argc, char** argv) {

//   Tx tx;
//   //Kinect k;
//   //XBoxControllerManager xbcm;
//   PhysicsModel pm;

//   pm.init();

//   int count = 0;
//   bool end = false;
//   char com;
 
//   /* Start curses mode */
//   initscr();
//   clear();
//   noecho();
//   cbreak();
//   refresh();

//   com = getch();

//   tx.setThrust(160);

//   /* Main loop of program */
//   while(!end) {
//     tx.sendCommand(com, true);
//     pm.sendCommand(com);

//     /*
//     xbcm.GetInput(ch); -- yet to be written
//     k.query(pm.kinX,pm.kinY, pm.kinZ); // yet to test
//     pm.rectify(); // yet to test (may need a lot)
//     */

//     usleep(29000); // Wait ~thirtieth second

//     clrtoeol();

//     com = getch();
//     clear();
//     printw("Char input: %d = %c \n", com, com);
//     refresh();
//     flushinp();
    
//     if(com == '0') {
//       end = true;
//     }
    
//     count++;

//     /*
//     if(count % 5 == 0) {
//       tx.cancel();
//     }
//     */
//   }

//   endwin();

//   return 0;
// }
