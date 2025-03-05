#include "InverseMotorModel.h"
#include "MotorController.h"
#include "utils/Utils.h"

void InverseMotorModel::setReference(Point new_pos_ref) {
    pos_ref = new_pos_ref;
}

void InverseMotorModel::update(MotorController &motorControllerLeft, MotorController &motorControllerRight, double theta) {
    double v_star = cos(theta) * pos_ref.x + sin(theta) * pos_ref.y;
    double w_star = -1 / LENGTH * sin(theta) * pos_ref.x + 1 / LENGTH * cos(theta) * pos_ref.y;

    double vd_star = v_star + WIDTH * w_star;
    double vg_star = v_star - WIDTH * w_star;

    motorControllerLeft.setReference(vg_star);
    motorControllerRight.setReference(vd_star);
}