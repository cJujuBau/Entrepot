#include "../include/Utils.h"

// Function to convert a value to PWM
int convertToPWM(const int value) {
    // Ensure the value is within the PWM range
    if (value < 0) {
        return 0;
    } else if (value * V2PWM > PWM_MAX) {
        return PWM_MAX;
    }
    return int(value * V2PWM);
}

float myAbs(const float value){
    return (value >= 0) ? value : -value;
}

float distance(const Point pos1, const Point pos2){
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y,2));
}