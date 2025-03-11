#include "../include/InverseMotorModel.h"

InverseMotorModel::InverseMotorModel(double Kx, double Ky) : pos_ref(Point(0, 0)), Kx(Kx), Ky(Ky), Vd_star(0), Vg_star(0) {}

void InverseMotorModel::setReference(const Point new_pos_ref, const Point robot_pos) {
    pos_ref.x = Kx * (new_pos_ref.x - robot_pos.x);
    pos_ref.y = Ky * (new_pos_ref.y - robot_pos.y);
}

void InverseMotorModel::update(const double theta) {
    const double v_star = cos(theta) * pos_ref.x + sin(theta) * pos_ref.y;
    const double w_star = -1. / LENGTH * sin(theta) * pos_ref.x + 1. / LENGTH * cos(theta) * pos_ref.y;

    Vd_star = v_star + WIDTH * w_star;
    Vg_star = v_star - WIDTH * w_star;
    //Serial.print("v_star = "); Serial.print(v_star); Serial.print("; w_star = "); Serial.println(w_star);
    //Serial.print("Vd_star = "); Serial.print(Vd_star); Serial.print("; Vg_star = "); Serial.println(Vg_star);
}

double InverseMotorModel::getVdStar(){
    return Vd_star;
}

double InverseMotorModel::getVgStar(){
    return Vg_star;
}