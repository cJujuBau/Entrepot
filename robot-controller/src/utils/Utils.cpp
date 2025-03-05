#include "Utils.h"

// Function to convert a value to PWM
int convertToPWM(int value) {
    // Ensure the value is within the PWM range
    if (value < 0) {
        return 0;
    } else if (value > 255) {
        return 255;
    }
    return value;
}

float abs(float value){
    return (value >= 0) ? value : -value;
}