#include "pollthread.h"
#include <QDebug>
#include <vector>

PollThread::PollThread(Monitor* _mon):mon(_mon){};

void PollThread::poll(){

    std::vector<float> location(3);
    std::vector<float> previous(3);

    while(true){

       mon->get_location(location);

       if(location != previous){
          setNewLoc(location);
          previous = location;
       }
    }
}
