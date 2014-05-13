#ifndef DATATYPES_H
#define DATATYPES_H

#include <math.h>
#include <QDebug>

struct Location
{
    double X, Y, Z;
    void setValues(double _x, double _y, double _z);

    //constructor
    Location();

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


struct kinect_frustum
{
    double near;
    double far;
    double ver_angle;
    double hor_angle;
    double left; //left near edge
    double right; //right near edge
    double top; //top near edge
    double bottom; //bottom near edge

    //constructor
    kinect_frustum(double _near, double _far);

    //check if a point is inside frustum
    bool inside(Location p);
};

#endif

