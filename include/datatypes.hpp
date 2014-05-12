#ifndef DATATYPES_H
#define DATATYPES_H
struct Location {
    double X, Y, Z;
    void setValues(double _x, double _y, double _z);

    //operators
    Location& operator+=(const Location& rhs);
    Location& operator-=(const Location& rhs);
    Location& operator*=(const Location& rhs);
    Location& operator/=(const Location& rhs);
    bool operator==(const Location& rhs) const;
    bool operator!=(const Location& rhs) const;

    double& operator[](int idx);
    double operator[](int idx) const;
};
#endif
