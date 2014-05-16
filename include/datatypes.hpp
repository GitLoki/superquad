#ifndef DATATYPES_H
#define DATATYPES_H

#include <math.h>
#include <iostream>
//#include <QDebug>

struct Location
{
    double X, Y, Z;
    void setValues(double _x, double _y, double _z);

    //constructor
    Location();
    Location(double x, double y, double z);

    // note - maximum is required to hold positive values
    void limit(Location maximum);

    //arithmetic operators
    Location& operator+=(const Location& rhs);
    Location& operator-=(const Location& rhs);
    Location& operator*=(const Location& rhs);
    Location& operator/=(const Location& rhs);

    //comparison operators
    bool operator==(const Location& rhs) const;
    bool operator!=(const Location& rhs) const;

    //array indexing
    double& operator[](int idx);
    double operator[](int idx) const;

    //constant mult/div
    Location& operator*=(double d);
    Location& operator/=(double d);
};


// operator overloading
Location operator+(Location lhs, const Location& rhs);
Location operator-(Location lhs, const Location& rhs);
Location operator*(Location lhs, const Location& rhs);
Location operator/(Location lhs, const Location& rhs);
Location operator*(Location l, double d);
Location operator*(double d,Location l);
Location operator/(Location l, double d);
std::ostream& operator<<(std::ostream& os, const Location& l);

struct KinectFrustum
{
    double near;
    double far;
    double vertAngle;
    double horzAngle;
    double left; //left near edge
    double right; //right near edge
    double top; //top near edge
    double bottom; //bottom near edge

    //constructor
    KinectFrustum(double _near, double _far);

    //check if a point is inside frustum
    bool inside(Location p);
};

#endif

