struct Location {
    double X, Y, Z;
    
    void setValues(double _x, double _y, double _z) {
        X = _x;
	    Y = _y;
	    Z = _z;
    }

    Location(double _x, double _y, double _z) {
        X = _x;
        Y = _y;
        Z = _z;
    }

    // note - maximum is required to hold positive values
    void limit(Location maximum) {
        if (this.X > maximum.X)
            this.X = maximum.X;
        if (this.X < (maximum.X * -1)
            this.X = maximum.X * -1;

        if (this.Y > maximum.Y)
            this.Y = maximum.Y;
        if (this.Y < (maximum.Y * -1)
            this.Y = maximum.Y * -1;

        if (this.Z > maximum.Z)
            this.Z = maximum.Z;
        if (this.Z < (maximum.Z * -1)
            this.Z = maximum.Z * -1;
    }
};

Location& operator=(Location other) {
    this.X = other.X;
    this.Y = other.Y;
    this.Z = other.Z;
    return *this;
}
