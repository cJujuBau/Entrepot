#include "../include/MotorController.h"

MotorController::MotorController(double Km, double Ki, double ref) : Km(Km), Ki(Ki), ref(ref) {}

void MotorController::setReference(const double ref) {
    this->ref = ref;
}

void MotorController::updateOutput(Motor &motor) {
    motor.setVoltage(this->ref / Km); // pas encore d'intégrateur implémenté
}