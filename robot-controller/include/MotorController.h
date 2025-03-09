#pragma once

#include "Motor.h"

class MotorController {
public:
    MotorController(double Km, double Ki, double ref);
    void setReference(double ref);
    void updateOutput(Motor &motor);

private:
    double Km;
    double Ki;
    double ref;
};