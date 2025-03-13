#include "Utils.h"
#include "Arduino.h"

class Trajectory {
public:
    Trajectory();

    Point computeTraj(float x, float t);
    void computeParameters(const float qi[3], const float qf[3]); // x, y, theta initial et final


private:
    float xi, yi, thetai;
    float xf, yf, thetaf;

    float ti, tf;

    float a0, a1, a2, a3; // coefficients associés à la trajectoire pour x = g(t)
    float b0, b1, b2, b3; // coefficients associés à la trajectoire pour y = f(x)
};