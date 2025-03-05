#include "Utils.h"

// Function to convert a value to PWM
int convertToPWM(int value) {
    // Ensure the value is within the PWM range
    if (value < 0) {
        return 0;
    } else if (value > PWM_MAX) {
        return PWM_MAX;
    }
    return value * V2PWM;
}

float abs(float value){
    return (value >= 0) ? value : -value;
}