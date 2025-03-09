#pragma once

#include "Utils.h"
#include "MotorController.h"
#include "Arduino.h"
#include <math.h>

// Réalise la conversion de 2 vitesses de référence x_ref_dot et y_ref_dot en 2 consignes de vitesse pour les moteurs
class InverseMotorModel {
public:
    InverseMotorModel(double Kx, double Ky);

    void setReference(const Point new_vel_ref);
    void update(const double theta);
    
    double getVdStar();
    double getVgStar();

private:
    Point vel_ref;
    double Kx;
    double Ky;
    double Vd_star;
    double Vg_star;
};