#include "../include/datatypes.hpp"

void Location::setValues(double _x, double _y, double _z) {
    X = _x;
    Y = _y;
    Z = _z;
}

//Addition
Location& Location::operator+=(const Location& rhs)
{
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;

    return *this;
}

Location operator+(Location lhs, const Location& rhs)
{
  lhs += rhs;
  return lhs;
}

//Subtraction
Location& Location::operator-=(const Location& rhs)
{
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;

    return *this;
}

Location operator-(Location lhs, const Location& rhs)
{
  lhs -= rhs;
  return lhs;
}

//Multiplication
Location& Location::operator*=(const Location& rhs)
{
    X *= rhs.X;
    Y *= rhs.Y;
    Z *= rhs.Z;

    return *this;
}

Location operator*(Location lhs, const Location& rhs)
{
  lhs *= rhs;
  return lhs;
}

//Division
Location& Location::operator/=(const Location& rhs)
{
    X /= rhs.X;
    Y /= rhs.Y;
    Z /= rhs.Z;

    return *this;
}

Location operator/(Location lhs, const Location& rhs)
{
  lhs /= rhs;
  return lhs;
}

//Comparison
bool Location::operator==(const Location& rhs) const {
    return(this->X == rhs.X && this->Y == rhs.Y && this->Z == rhs.Z);
}

bool Location::operator!=(const Location& rhs) const {
    return(!(*this == rhs));
}


//Array subscription
double& Location::operator[](int idx){
    switch(idx){
    case 0:
        return X;
        break;
    case 1:
        return Y;
        break;
    case 2:
        return Z;
        break;
    }
}

double Location::operator[](int idx) const{
    switch(idx){
    case 0:
        return X;
        break;
    case 1:
        return Y;
        break;
    case 2:
        return Z;
        break;
    }
}
