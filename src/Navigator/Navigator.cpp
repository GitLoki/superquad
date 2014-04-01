#include "../../include/Navigator/Navigator.hpp"

Navigator::findDistance(Location* A, Location* B) {
    double diffX = A->X - B->X,
           diffY = A->Y - B->Y,
           diffY = A->Z - B->Z;

    double diffXYZ = sqrt(pow(diffX, 2) + pow(diffY, 2) + pow(diffZ, 2));
    return diffXYZ;
}

int goToWaypoint(Location* Waypoint) {

    double startclock, cumulativeDistance, currentdistance;

    pid.updateDestination(Waypoint);

    do {
        cumulativeDistance=0;
        startclock = clock();
	while (clock() - startclock < loop_time) {
	    pid.goToDestination(&currentLocation);
	    cumulativeDistance += findDistance(Waypoint, &currentLocation);
	}
    }

    while {
         cumulativeDistance > epsilon;
    }

    return 0;
}

int Navigator::followWayPoints(Location* Waypoints, int NumberOfWaypoints) {
    for (int i = 0 ; i < numberOfWaypoints ; i++) {
        goToWaypoint(Waypoints[i]);
    }  
    return 0;
}
