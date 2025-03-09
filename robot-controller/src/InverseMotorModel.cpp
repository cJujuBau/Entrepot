#include "../include/InverseMotorModel.h"

InverseMotorModel::InverseMotorModel(double Kx, double Ky) : vel_ref(Point(0, 0)), Kx(Kx), Ky(Ky), Vd_star(0), Vg_star(0) {}

void InverseMotorModel::setReference(const Point new_vel_ref) {
    vel_ref = new_vel_ref;
}

void InverseMotorModel::update(const double theta) {
    const double v_star = cos(theta) * vel_ref.x + sin(theta) * vel_ref.y;
    const double w_star = -1 / LENGTH * sin(theta) * vel_ref.x + 1 / LENGTH * cos(theta) * vel_ref.y;

    Vd_star = v_star + WIDTH * w_star;
    Vg_star = v_star - WIDTH * w_star;
}