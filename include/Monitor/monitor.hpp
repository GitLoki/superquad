#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <pthread.h>

#include "../datatypes.hpp"


//struct that holds a bool for each variable the GUI can change (i.e. not location)
//If this has been changed, set that bool to true.
//these are only changed from within mutex functions in this object

struct ChangeTracker
{
    bool target,lights,land,stop,snap;
};


class Monitor {

private:

    //shared variables
    Location target;
    Location location;
    bool lights;
    bool land_quad;
    bool emergency;
    bool snap_lim;

    //mutex
    pthread_mutex_t monitor_lock;


public:

    ChangeTracker changed;

    //constructor
    Monitor();

    //setters
    void set_target(Location &values);
    void set_location(Location &values);
    void lightswitch(bool set_to);
    void land();
    void stop();
    void snap(bool set_to);

    //getters
    void get_target(Location &values);
    void get_location(Location &values);
    bool get_light();
    bool get_land();
    bool get_stop();
    bool get_snap();
};

#endif
