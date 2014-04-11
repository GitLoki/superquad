#include "../../include/Monitor/monitor.hpp"
#include <iostream>

//////To add a new shared variable//////

//add private variable to (hpp)
//create a mutex for that variable (hpp)
//initialise that mutex in constructor (cpp)
//create protected get & set functions as required (hpp & cpp)


Monitor::Monitor(): target(3,0), location(3,0), lights(false),
    target_lock(PTHREAD_MUTEX_INITIALIZER), location_lock(PTHREAD_MUTEX_INITIALIZER), lights_lock(PTHREAD_MUTEX_INITIALIZER)
    {};

void Monitor::set_target(std::vector<float> &values){
    pthread_mutex_lock (&target_lock);
    target = values;
    pthread_mutex_unlock (&target_lock);
}

void Monitor::set_location(std::vector<float> &values){
    pthread_mutex_lock (&location_lock);
    location = values;
    pthread_mutex_unlock (&location_lock);
}

void Monitor::lightswitch(){
    pthread_mutex_lock (&location_lock);
    lights = !lights;
    pthread_mutex_unlock (&location_lock);
}


void Monitor::get_target(std::vector<float> &values){
    pthread_mutex_lock (&target_lock);
    values = target;
    pthread_mutex_unlock (&target_lock);
}

void Monitor::get_location(std::vector<float> &values){
    pthread_mutex_lock (&location_lock);
    values = location;
    pthread_mutex_unlock (&location_lock);
}

bool Monitor::get_light(){
    pthread_mutex_lock (&lights_lock);
    return lights;
    pthread_mutex_unlock (&lights_lock);
}
