#include "../include/datatypes.hpp"

Location::Location():X(0),Y(0),Z(0){};

Location::Location(double x, double y, double z) {
    setValues(x, y, z);
}

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
double& Location::operator[](int idx)
{
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

double Location::operator[](int idx) const
{
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
    return 0;
}

//multiply by a double
Location& Location::operator*=(double d)
{
    X *= d;
    Y *= d;
    Z *= d;

    return *this;
}

Location operator*(Location l, double d)
{
    l *= d;
    return l;
}

Location operator*(double d,Location l)
{
    l *= d;
    return l;
}

//divide by a double
Location& Location::operator/=(double d)
{
    X /= d;
    Y /= d;
    Z /= d;

    return *this;
}

Location operator/(Location l, double d)
{
    l /= d;
    return l;
}

// note - maximum is required to hold positive values
void Location::limit(Location maximum) {
    if (this->X > maximum.X)
        this->X = maximum.X;
    if (this->X < (maximum.X * -1))
        this->X = maximum.X * -1;

    if (this->Y > maximum.Y)
        this->Y = maximum.Y;
    if (this->Y < (maximum.Y * -1))
        this->Y = maximum.Y * -1;

    if (this->Z > maximum.Z)
        this->Z = maximum.Z;
    if (this->Z < (maximum.Z * -1))
        this->Z = maximum.Z * -1;
}


std::ostream& operator<<(std::ostream& os, const Location& l)
{
  os << "[" << l.X << "," << l.Y << "," << l.Z << "]";
  return os;
}


KinectFrustum::KinectFrustum(double _near, double _far)
{
    near = _near;
    far = _far;
    vertAngle = (43.0/2.0)*(M_PI/180.0);
    horzAngle = (57.0/2.0)*(M_PI/180.0);

    left = near*tan(horzAngle);
    right = -left;
    top = near*tan(vertAngle);
    bottom = -top;
}

bool KinectFrustum::inside(Location p)
{

    if(p.Z < near || p.Z > far)
        return false;

    double bottom_dot, top_dot, left_dot, right_dot;

    //dot products with plane normals
    bottom_dot = (p.Y*near - p.Z*bottom);
    top_dot = (p.Z*top - p.Y*near);
    left_dot = (p.Z*left - p.X*near);
    right_dot = (p.X*near - p.Z*right);

    if(bottom_dot < 0 || top_dot < 0 || left_dot < 0 || right_dot < 0)
        return false;
    return true;
}
