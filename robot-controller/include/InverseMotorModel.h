#pragma once

#include "Utils.h"
#include "MotorController.h"
#include "Arduino.h"
#include <math.h>


class InverseMotorModel {
public:
    InverseMotorModel(Point initial_pos_ref, double Kx, double Ky) : pos_ref(initial_pos_ref), Kx(Kx), Ky(Ky) {}

    void setReference(const Point new_pos_ref);
    void update(MotorController &motorControllerLeft, MotorController &motorControllerRight, const double theta);
    

private:
    Point pos_ref;
    double Kx;
    double Ky;
};