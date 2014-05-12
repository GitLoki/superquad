#include "../../include/Monitor/monitor.hpp"
#include <iostream>

//////To add a new shared variable//////

//add private variable to (hpp)
//create a mutex for that variable (hpp)
//initialise that mutex in constructor (cpp)
//create protected get & set functions as required (hpp & cpp)


//constructor
Monitor::Monitor(): target(3,0), location(3,0), lights(true), land_quad(false), emergency(false),
    target_lock(PTHREAD_MUTEX_INITIALIZER), location_lock(PTHREAD_MUTEX_INITIALIZER), lights_lock(PTHREAD_MUTEX_INITIALIZER),
    land_lock(PTHREAD_MUTEX_INITIALIZER), emergency_lock(PTHREAD_MUTEX_INITIALIZER)
    {};


//setters
void Monitor::set_target(std::vector<float> &values)
{
    pthread_mutex_lock (&target_lock);
    target = values;
    pthread_mutex_unlock (&target_lock);
}

void Monitor::set_location(std::vector<float> &values)
{
    pthread_mutex_lock (&location_lock);
    location = values;
    pthread_mutex_unlock (&location_lock);
}

void Monitor::lightswitch()
{
    pthread_mutex_lock (&lights_lock);
    lights = !lights;
    pthread_mutex_unlock (&lights_lock);
}

void Monitor::land()
{
    pthread_mutex_lock (&land_lock);
    land_quad = true;
    pthread_mutex_unlock (&land_lock);
}

void Monitor::stop()
{
    pthread_mutex_lock (&emergency_lock);
    emergency = true;
    pthread_mutex_unlock (&emergency_lock);
}


//getters
void Monitor::get_target(std::vector<float> &values)
{
    pthread_mutex_lock (&target_lock);
    values = target;
    pthread_mutex_unlock (&target_lock);
}

void Monitor::get_location(std::vector<float> &values)
{
    pthread_mutex_lock (&location_lock);
    values = location;
    pthread_mutex_unlock (&location_lock);
}

bool Monitor::get_light()
{
    pthread_mutex_lock (&lights_lock);
    return lights;
    pthread_mutex_unlock (&lights_lock);
}

bool Monitor::get_land()
{
    pthread_mutex_lock (&land_lock);
    return land_quad;
    pthread_mutex_unlock (&land_lock);
}

bool Monitor::get_stop()
{
    pthread_mutex_lock (&emergency_lock);
    return emergency;
    pthread_mutex_unlock (&emergency_lock);
}
