#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <vector>
#include <pthread.h>

//////To add a new shared variable//////

//add private variable to (hpp)
//create a mutex for that variable (hpp)
//initialise that mutex in constructor (cpp)
//create protected get & set functions as required (hpp & cpp)

class Monitor {

private:

    //shared variables
    std::vector<float> target;
    std::vector<float> location;
    bool lights;
    bool land_quad;
    bool emergency;

    //mutexes
    pthread_mutex_t target_lock;
    pthread_mutex_t location_lock;
    pthread_mutex_t lights_lock;
    pthread_mutex_t land_lock;
    pthread_mutex_t emergency_lock;

public:

    //constructor
    Monitor();

    //setters
    void set_target(std::vector<float> &values);
    void set_location(std::vector<float> &values);
    void lightswitch();
    void land();
    void stop();

    //getters
    void get_target(std::vector<float> &values);
    void get_location(std::vector<float> &values);
    bool get_light();
    bool get_land();
    bool get_stop();
};

#endif
