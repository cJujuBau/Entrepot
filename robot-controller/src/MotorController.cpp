#include "../include/MotorController.h"

MotorController::MotorController(const double Km, const double Ki) : Km(Km), Ki(Ki), controlledVoltage(0), integralError(0), lim_max(6.), lim_min(-6.) {}

void MotorController::setControlledVoltage(const double refSpeed, const double actualSpeed) {
    const double error = refSpeed - actualSpeed;
    double lim_max_integ = 0; double lim_min_integ = 0;

    this->integralError += Ki * error * DT * MS_TO_S;
    const double proportional = error * Km;

    if (lim_max > proportional) lim_max_integ = lim_max - proportional;
    else lim_max_integ = 0.;
    if (lim_min < proportional) lim_min_integ = lim_min - proportional;
    else lim_min_integ = 0.;

    if (integralError > lim_max_integ) integralError = lim_max_integ;
    if (integralError < lim_min_integ) integralError = lim_min_integ;

    double result = integralError + proportional;
    //double result = proportional;
    this->controlledVoltage = constrain(result, lim_min, lim_max);
}

double MotorController::getControlledVoltage() {
    return controlledVoltage;
}