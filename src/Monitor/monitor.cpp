#include "../../include/Monitor/monitor.hpp"
#include <iostream>

//constructor
Monitor::Monitor(): lights(true), land_quad(false), emergency(false), snap_lim(false),
    monitor_lock(PTHREAD_MUTEX_INITIALIZER)
    {};


//setters
void Monitor::set_target(Location &values)
{
    pthread_mutex_lock (&monitor_lock);
    target = values;
    changed.target = true;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::set_location(Location &values)
{
    pthread_mutex_lock (&monitor_lock);
    location = values;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::lightswitch(bool set_to)
{
    pthread_mutex_lock (&monitor_lock);
    lights = set_to;
    changed.lights = true;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::land()
{
    pthread_mutex_lock (&monitor_lock);
    land_quad = true;
    changed.land = true;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::stop()
{
    pthread_mutex_lock (&monitor_lock);
    emergency = true;
    changed.stop = true;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::snap(bool set_to)
{
    pthread_mutex_lock (&monitor_lock);
    snap_lim = set_to;
    changed.snap = true;
    pthread_mutex_unlock (&monitor_lock);
}

//getters
void Monitor::get_target(Location &values)
{
    pthread_mutex_lock (&monitor_lock);
    values = target;
    changed.target = false;
    pthread_mutex_unlock (&monitor_lock);
}

void Monitor::get_location(Location &values)
{
    pthread_mutex_lock (&monitor_lock);
    values = location;
    pthread_mutex_unlock (&monitor_lock);
}

bool Monitor::get_light()
{
    bool tempLights;

    pthread_mutex_lock (&monitor_lock);
    tempLights = lights;
    changed.lights = false;
    pthread_mutex_unlock (&monitor_lock);

    return tempLights;
}

bool Monitor::get_land()
{
    bool tempLand;

    pthread_mutex_lock (&monitor_lock);
    tempLand = land_quad;
    changed.land = false;
    pthread_mutex_unlock (&monitor_lock);

    return tempLand;
}

bool Monitor::get_stop()
{
    bool tempStop;

    pthread_mutex_lock (&monitor_lock);
    tempStop = emergency;
    changed.stop = false;
    pthread_mutex_unlock (&monitor_lock);

    return tempStop;
}

bool Monitor::get_snap()
{
    bool tempSnap;

    pthread_mutex_lock (&monitor_lock);
    tempSnap = snap_lim;
    changed.snap = false;
    pthread_mutex_unlock (&monitor_lock);

    return tempSnap;
}
