#pragma once

#include "utils/Utils.h"
#include "MotorController.h"

class InverseMotorModel {
public:
    InverseMotorModel(struct Point initial_pos_ref, double Kx, double Ky) : pos_ref(initial_pos_ref), Kx(Kx), Ky(Ky) {}

    void setReference(struct Point new_pos_ref);
    void update(MotorController &motorControllerLeft, MotorController &motorControllerRight, double theta);
    

private:
    struct Point pos_ref;
    double Kx;
    double Ky;
};