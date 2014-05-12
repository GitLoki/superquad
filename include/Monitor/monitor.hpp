#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <vector>
#include <pthread.h>

class Monitor {

private:

    //shared variables
    std::vector<float> target;
    std::vector<float> location;
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
    void set_target(std::vector<float> &values);
    void set_location(std::vector<float> &values);
    void lightswitch(bool set_to);
    void land();
    void stop();
    void snap(bool set_to);

    //getters
    void get_target(std::vector<float> &values);
    void get_location(std::vector<float> &values);
    bool get_light();
    bool get_land();
    bool get_stop();
    bool get_snap();
};

#endif
