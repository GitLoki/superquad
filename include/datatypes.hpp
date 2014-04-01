struct Location {
    double X, Y, Z;
    Location() {
        X = Y = Z = 0;
    }
    Location(double _x, double _y, double _z) : X(_x), Y(_y), Z(_z) {};
};
