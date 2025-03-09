#include "../include/Robot.h"

Robot::Robot(Point pos, Motor& motorLeft, Motor& motorRight, MotorController& motorControllerLeft, 
             MotorController& motorControllerRight, InverseMotorModel& inverseMotorModel, double theta) 
: pos(pos), motorLeft(motorLeft), motorRight(motorRight), theta(theta), v(0), w(0), 
  motorControllerLeft(motorControllerLeft), motorControllerRight(motorControllerRight), 
  inverseMotorModel(inverseMotorModel) {}

void Robot::updateState(){
    double Vg = motorLeft.getSpeed();
    double Vd = motorRight.getSpeed();
    v = (Vg + Vd)/2;
    w = (Vd - Vg)/(2 * WIDTH);
    
    double dx = cos(theta) * v - LENGTH * sin(theta) * w;
    double dy = sin(theta) * v + LENGTH * cos(theta) * w;
    
    theta += w * DT * MS_TO_S;
    pos.x += DT * MS_TO_S * dx;
    pos.y += DT * MS_TO_S * dy;
}

void Robot::readMarvelmind(){
}

double Robot::getTheta(){
    return this->theta;
}