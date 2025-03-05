#pragma once

#include "utils/Utils.h"
#include "Robot.h"

class InverseMotorModel {
public:
    InverseMotorModel(Point initial_pos_ref, double Kx, double Ky) : pos_ref(initial_pos_ref), Kx(Kx), Ky(Ky) {}

    void setReference(Point new_pos_ref);
    void update(MotorController &motorControllerLeft, MotorController &motorControllerRight, double theta);

private:
    Point pos_ref;
    double Kx;
    double Ky;
};