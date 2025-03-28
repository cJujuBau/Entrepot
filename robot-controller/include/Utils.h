#pragma once

#include "Arduino.h"

const double PWM_MAX = 255;  // max value of duty cycle that can be sent
const double V_MAX = 12.0;   // max voltage that can be sent through the H-bridge
const double V2PWM = PWM_MAX / V_MAX;
const double DT = 7.0;  // step time in ms

const float WIDTH = (200./2.); // width/2 of the robot in mm (distance between wheels)
const float LENGTH = (220./2.); // length/2 of the robot in mm (front to back)

const double REDUCTION_RATE = 46.0;
const double WHEEL_RADIUS = 30.0;
const double MS_TO_S = 1e-3;
const double PULSE_PER_TURN = 8.;

const int FORWARD = 1;
const int BACKWARD = -1;

// Structure to represent a point
struct Point {
    float x;
    float y;

    Point(float x = 0, float y = 0) : x(x), y(y) {}
};

// Function to convert a value to PWM
int convertToPWM(const int value);

// Return the absolute value of a number
float myAbs(const float value);

float distance(const Point pos1, const Point pos2);