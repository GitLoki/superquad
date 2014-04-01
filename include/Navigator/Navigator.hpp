#ifndef NAVIGATE_HPP
#define NAVIGATE_HPP
 
#include <time>
#include "../PID/PID.hpp"
#include "../Kinect/Kinect.hpp"
#include "../Tx/Tx.hpp"
#include "../datatypes.hpp"

class Navigator {
private:
    static const double epsilon = 5;
    static const double loop_time = 1000; 
    Location* waypoints;
    double currentDistance;
    Location currentlocation; 
    int goToWaypoint(Location* Waypoint);
    double findDistance(Location* A, Location* B);
public:
    Navigator(){};
    int followWaypoints(Location* Waypoints, int NumberOfWaypoints);
};

#endif
