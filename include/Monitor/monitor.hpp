#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <vector>

class Monitor {

private:
    std::vector<float> target;
    std::vector<float> location;
    bool lights;

public:

    Monitor();

    //setters
    void set_target(std::vector<float> values);
    void set_location(std::vector<float> values);
    void lightswitch();

    //getters
    void get_target(std::vector<float> values);
    void get_location(std::vector<float> values);
    bool get_light();
};

#endif
