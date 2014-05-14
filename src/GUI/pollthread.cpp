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

    //FOR TESTING - DELETE LATER
    int i = 0;
    Location target;
    //FOR TESTING - DELETE LATER

    while(true){

        delay(20);

        mon->get_location(location);

        if(location != previous){
            setNewLoc(location);
            previous = location;
        }

        //FOR TESTING - DELETE LATER
        i++;

        if(i>100000){

            mon->get_target(target);

            for(int j=0;j<3;j++){
                if(location[j] < target[j])
                    location[j] += 10;
                else if (location[j] > target[j])
                    location[j] -= 10;
            }

            mon->set_location(location);

            i=0;
        }

        //FOR TESTING - DELETE LATER
    }
}

//function to pause for some number of milliseconds
void PollThread::delay(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
