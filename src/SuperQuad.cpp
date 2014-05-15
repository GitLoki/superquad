#include "GUI/gui_interface.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>
#include "GUI/pollthread.h"
#include "../include/Monitor/monitor.hpp"
#include "../include/config.hpp"
#include <iostream>
#include "../include/datatypes.hpp"
#include "../include/Tx/Tx.hpp"
#include "../include/Kinect/Kinect.hpp"
#include "../include/PhysicsModel/PhysicsModel.hpp"
#include "../include/Controller/boost_xbox_controller.hpp"
//#include "../include/PID/PID.hpp"
#include "../include/CascadeControl/CascadeControl.hpp"
#include "../include/CascadeControl/AccelerationControl.hpp"
#include "../include/CascadeControl/VelocityControl.hpp"
#include "../include/config.hpp"
#include <pthread.h>
#include <assert.h>
#include <ctime>
#include <unistd.h>

using namespace std;

// null location object for testing kinect results against
Location nullLoc(0,0,0);

// set point for velocity calibration
Location v_setPoint(0, 0, 150);

// data objects for velocity control
Location v_K(0.1, 0.1, 1.0);
Location v_snapLimit(20, 20, 60);

// data objects for acceleration control
Location a_K(0.1, 0.1, 0.5);
Location a_jerkLimit(10, 10, 30);

struct argstruct {
    int argcs;
    char **argvs;
    Monitor *m;
};

void *contfun(void *argument);

void *guifun(void *argument);

int main (int argc, char** argv) {

    int ret_code;

    argstruct arguments;

    Monitor mon;

    arguments.argcs = argc;
    arguments.argvs = argv;
    arguments.m = &mon;

    pthread_t controlthread, guithread;

    //have both threads call their respective functions
    ret_code = pthread_create(&controlthread, NULL, contfun, &arguments);
    assert(ret_code == 0);

    ret_code = pthread_create(&guithread, NULL, guifun, &arguments);
    assert(ret_code == 0);

    //wait for both threads to finish
    ret_code = pthread_join(guithread, NULL);
    assert(ret_code == 0);

    ret_code = pthread_join(controlthread, NULL);
    assert(ret_code == 0);

    return 0;

}

void *contfun(void *argument){

  //argstruct *args = (argstruct*) argument;

    Location currentLocation, destination, flightVariables;
    
    std::cout << "Initialising Kinect..." << std::endl;
    Kinect* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    
    std::cout << "Initialising Transmitter..." << std::endl;
    Tx* tx = new Tx;
    Location trim;
    tx->getValues(&trim);

    std::cout << "Initialising Control Structure..." << std::endl;
    CascadeControl* cascadeControl = new CascadeControl(trim, v_snapLimit, v_K,
                                                        a_jerkLimit, a_K);
    cascadeControl->changeVelocitySetPoint(v_setPoint);

    std::cout << "Transmitter Initialised. Counting down..." << std::endl;
    for ( int i = COUNTDOWN ; i >= 0 ; i-- ) {
        std::cout << i << std::endl;
        usleep(ONE_SECOND);
    }
    std::cout << std::endl << "Liftoff" << std::endl;

    //double oldtime, newtime;
    //oldtime = clock();
    timespec oldTime, newTime;
    clock_gettime(CLOCK_REALTIME, &oldTime);

    while (true) {
      
      // query kinect until a valid reading is acquired
      do currentLocation = kinect->query();
      while (currentLocation == nullLoc);

      // query 
      flightVariables = cascadeControl->query(currentLocation, v_snapLimit);
      tx->setValues(flightVariables);

      // Log time and settings for diagnostic reasons
      //newtime = clock();
      clock_gettime(CLOCK_REALTIME, &newTime);
      std::cout << "time nanoseconds: " << (newTime.tv_sec - oldTime.tv_sec) << std::endl;
      oldTime.tv_sec = newTime.tv_sec;
      oldTime.tv_nsec = newTime.tv_nsec;
      std::cout << "Kinect: " << currentLocation << std::endl;
      std::cout << "FlightVars: " << flightVariables << std::endl << std::endl;

      // wait 1/FPS seconds
      usleep(ONE_SECOND / FPS);
    }
    return NULL;
}

void *guifun(void *argument){

    argstruct *args = (argstruct*) argument;

    QApplication quadcop(args->argcs, args->argvs);
    GUI_interface w(args->m);

    w.show();
    
    quadcop.exec();

    return NULL;
}

//////////////////////////
// Old PID testing loop://
//////////////////////////
/*
{
   Location currentLocation, destination;
    currentLocation.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, 1400);

    std::cout << "Initialising Kinect..." << std::endl;
    Kinect* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    std::cout << "Initialising Transmitter..." << std::endl;
    Tx* tx = new Tx;
    std::cout << "Transmitter Initialised. Waiting for serial connection..." << std::endl;
    usleep(1000000 * COUNTDOWN);

    std::cout << "Waiting complete. Initiating automated control..." << std::endl;

    PID* pid = new PID(kinect, tx);

    // register signal SIGINT and signal handler  
    // signal(SIGINT, signalHandler);  

    while (pid->goToDestination(currentLocation)) {
      // usleep(1000000 / FPS);
}
 */
