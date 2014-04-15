struct Location {
    double X, Y, Z;
    void setValues(double _x, double _y, double _z) {
        X = _x;
	Y = _y;
	Z = _z;
    }
};

Location& operator=(Location other) {
    this.X = other.X;
    this.Y = other.Y;
    this.Z = other.Z;
    return *this;
}
