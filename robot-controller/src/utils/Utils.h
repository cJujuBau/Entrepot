#pragma once

#define PWM_MAX 255  // max value of duty cycle that can be sent
#define V_MAX 12.0   // max voltage that can be sent through the H-bridge
#define V2PWM PWM_MAX / V_MAX
#define DT 5.  // step time in ms

#define WIDTH 85 // width/2 of the robot in mm (distance between wheels)
#define LENGTH 115 // length/2 of the robot in mm (front to back)

#define REDUCTION_RATE 46.
#define WHEEL_RADIUS 30.
#define MS_TO_S 1e-3
#define PULSE_PER_TURN 8.

// Function to convert a value to PWM
int convertToPWM(int value);

// Return the absolute value of a number
float myAbs(float value);

// Structure to represent a point
struct Point {
    float x;
    float y;

    Point(float x = 0, float y = 0) : x(x), y(y) {}
};