#include "../../include/Monitor/monitor.hpp"
#include <iostream>

Monitor::Monitor(): target(3,0), location(3,0), lights(false), monitor_lock(PTHREAD_MUTEX_INITIALIZER){
};


void Monitor::set_target(std::vector<float> &values){
    pthread_mutex_lock (&monitor_lock);
    target = values;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::set_location(std::vector<float> &values){
    pthread_mutex_lock (&monitor_lock);
    location = values;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::lightswitch(){
    pthread_mutex_lock (&monitor_lock);
    lights = !lights;
    pthread_mutex_unlock (&monitor_lock);
}


void Monitor::get_target(std::vector<float> &values){
    pthread_mutex_lock (&monitor_lock);
    values = target;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::get_location(std::vector<float> &values){
    pthread_mutex_lock (&monitor_lock);
    values = location;
    pthread_mutex_unlock (&monitor_lock);
}

bool Monitor::get_light(){
    pthread_mutex_lock (&monitor_lock);
    return lights;
    pthread_mutex_unlock (&monitor_lock);
}
