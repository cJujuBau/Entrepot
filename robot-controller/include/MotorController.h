#pragma once

#include "Motor.h"

// Réalise la convertion d'une vitesse désirée pour le moteur en une tension
class MotorController {
public:
    MotorController(const double Km, const double Ki);
    void setControlledVoltage(const double refSpeed);
    double getControlledVoltage();

private:
    double Km;
    double Ki;
    double controlledVoltage;
};