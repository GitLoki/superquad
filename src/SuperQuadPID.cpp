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
#include "../include/PID/PID.hpp"
#include "../include/config.hpp"
#include <pthread.h>
#include <assert.h>
#include <ctime>
#include <unistd.h>

using namespace std;

// structure to hold arguments being passed to threads
struct argstruct {
    int argcs;
    char **argvs;
    Monitor *m;
};

// controller function to be run by own thread
void *contfun(void *argument);

// gui function to be run by own thread
void *guifun(void *argument);

int main (int argc, char** argv) {

    int ret_code;

    argstruct arguments;

    Monitor mon;

    arguments.argcs = argc;
    arguments.argvs = argv;
    arguments.m = &mon;

    pthread_t controlthread, guithread;

    // have both threads call their respective functions
    ret_code = pthread_create(&controlthread, NULL, contfun, &arguments);
    assert(ret_code == 0);

    ret_code = pthread_create(&guithread, NULL, guifun, &arguments);
    assert(ret_code == 0);

    // wait for both threads to finish
    ret_code = pthread_join(guithread, NULL);
    assert(ret_code == 0);

    ret_code = pthread_join(controlthread, NULL);
    assert(ret_code == 0);

    return 0;

}

void *contfun(void *argument){

   Location currentLocation, destination;
    currentLocation.setValues(XCENTRE, YCENTRE, 0.0);
    destination.setValues(XCENTRE, YCENTRE, 1400);

    std::cout << "Initialising Kinect..." << std::endl;
    Kinect* kinect = new Kinect;  
    std::cout << "Kinect Initialised." << std::endl;
    std::cout << "Initialising Transmitter..." << std::endl;
    Tx* tx = new Tx;
    std::cout << "Transmitter Initialised. Waiting for serial connection..." 
			  << std::endl;

    std::cout << "Transmitter Initialised. Waiting for serial connection...." 
	      << std::endl;
    for ( int i = COUNTDOWN ; i >= 0 ; i-- ) {
        std::cout << i << std::endl;
        usleep(ONE_SECOND);
    }
    std::cout << std::endl << "Liftoff!!!!" << std::endl;


    std::cout << "Waiting complete. Initiating automated control..." 
			  << std::endl;

    PID* pid = new PID(kinect, tx);

	// let PID controller take over
    while (pid->goToDestination(currentLocation)) {
      usleep(1000000 / FPS);
	}

    return NULL;
}

void *guifun(void *argument){

    argstruct *args = (argstruct*) argument;

	// initialise GUI and display it to the user
    QApplication quadcop(args->argcs, args->argvs);
    GUI_interface w(args->m);
    w.show();
    quadcop.exec();

    return NULL;
}
