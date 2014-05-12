#include "../../include/Monitor/monitor.hpp"
#include <iostream>

//////To add a new shared variable//////

//add private variable to (hpp)
//create a mutex for that variable (hpp)
//initialise that mutex in constructor (cpp)
//create protected get & set functions as required (hpp & cpp)


//constructor
Monitor::Monitor(): target(3,0), location(3,0), lights(true), land_quad(false), emergency(false), snap_lim(false),
    monitor_lock(PTHREAD_MUTEX_INITIALIZER)
    {};


//setters
void Monitor::set_target(std::vector<float> &values)
{
    pthread_mutex_lock (&monitor_lock);
    target = values;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::set_location(std::vector<float> &values)
{
    pthread_mutex_lock (&monitor_lock);
    location = values;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::lightswitch(bool set_to)
{
    pthread_mutex_lock (&monitor_lock);
    lights = set_to;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::land()
{
    pthread_mutex_lock (&monitor_lock);
    land_quad = true;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::stop()
{
    pthread_mutex_lock (&monitor_lock);
    emergency = true;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::snap(bool set_to)
{
    pthread_mutex_lock (&monitor_lock);
    snap_lim = set_to;
    pthread_mutex_unlock (&monitor_lock);
}



//getters
void Monitor::get_target(std::vector<float> &values)
{
    pthread_mutex_lock (&monitor_lock);
    values = target;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::get_location(std::vector<float> &values)
{
    pthread_mutex_lock (&monitor_lock);
    values = location;
    pthread_mutex_unlock (&monitor_lock);
}

bool Monitor::get_light()
{
    pthread_mutex_lock (&monitor_lock);
    return lights;
    pthread_mutex_unlock (&monitor_lock);
}

bool Monitor::get_land()
{
    pthread_mutex_lock (&monitor_lock);
    return land_quad;
    pthread_mutex_unlock (&monitor_lock);
}

bool Monitor::get_stop()
{
    pthread_mutex_lock (&monitor_lock);
    return emergency;
    pthread_mutex_unlock (&monitor_lock);
}

bool Monitor::get_snap()
{
    pthread_mutex_lock (&monitor_lock);
    return snap_lim;
    pthread_mutex_unlock (&monitor_lock);
}
