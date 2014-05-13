#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <pthread.h>

#include "../datatypes.hpp"

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
