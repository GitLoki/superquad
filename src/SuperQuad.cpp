//QT includes
#include "GUI/gui_interface.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>
#include "GUI/pollthread.h"
#include "../include/Monitor/monitor.hpp"

#include <iostream>
#include "../include/Tx/Tx.hpp"
#include "../include/Kinect/Kinect.hpp"
#include "../include/PhysicsModel/PhysicsModel.hpp"
#include "../include/Controller/boost_xbox_controller.hpp"
#include "../include/PID/PID.hpp"
#include <pthread.h>
#include <assert.h>

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
    //signal(SIGINT, signalHandler);  

    while (pid->goToDestination(currentLocation)) {
      //usleep(1000000 / FPS);
    }

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
