#include "../include/MotorController.h"

MotorController::MotorController(double Km, double Ki) : Km(Km), Ki(Ki), controlledVoltage(0) {}

void MotorController::setControlledVoltage(const double refSpeed) {
    this->controlledVoltage = refSpeed * Km; // pas encore d'intégrateur implémenté
}

double MotorController::getControlledVoltage() {
    return controlledVoltage;
}