#include "pollthread.h"
#include <QDebug>
#include <vector>
#include <QTime>
#include <QCoreApplication>

PollThread::PollThread(Monitor* _mon):mon(_mon){}

//endless loop checking whether location has updated.
void PollThread::poll(){

    Location location;
    Location previous;

    while(true){

        delay(20);

        mon->get_location(location);

        if(location != previous){
            set_new_loc(location);
            previous = location;
        }
    }
}

//function to pause for some number of milliseconds
void PollThread::delay(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
//Source: http://stackoverflow.com/a/11487434/3511582
