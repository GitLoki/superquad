// #include <iostream>
// #include <ncurses.h>
// #include "../include/Tx/Tx.hpp"
// #include "../include/Kinect/Kinect.hpp"
// #include "../include/PhysicsModel/PhysicsModel.hpp"
// #include "../include/Controller/boost_xbox_controller.hpp"

#include <pthread.h>
#include <assert.h>
#include "../include/Monitor/Monitor.hpp"

using namespace std;

void *contfun(void *argument);

void *guifun(void *argument);

int main (int argc, char** argv) {

    int ret_code;
    Monitor *m = new Monitor;
    
    pthread_t guithread, controlthread;

    //have both threads call their respective functions
    ret_code = pthread_create(&guithread,NULL,guifun, (void*) m);
    assert(ret_code == 0);

    ret_code = pthread_create(&controlthread,NULL,contfun, (void*) m);
    assert(ret_code == 0);

    //wait for both threads to finish
    ret_code = pthread_join(guithread, NULL);
    assert(ret_code == 0);
    
    ret_code = pthread_join(controlthread, NULL);
    assert(ret_code == 0);
    
    return 0;

}

void *contfun(void *argument){

    Monitor *mon = (Monitor*) argument;

    while(true);

    return NULL;
}

void *guifun(void *argument){

    Monitor *mon = (Monitor*) argument;

    QApplication a(void);
    GUI w(mon);
    w.show();
    a.exec();

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
