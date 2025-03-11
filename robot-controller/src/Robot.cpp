#include "../include/Robot.h"

Robot::Robot(Point pos, Motor& motorLeft, Motor& motorRight, MotorController& motorControllerLeft, 
             MotorController& motorControllerRight, InverseMotorModel& inverseMotorModel, double theta) 
: pos(pos), motorLeft(motorLeft), motorRight(motorRight), theta(theta), v(0), w(0), 
  motorControllerLeft(motorControllerLeft), motorControllerRight(motorControllerRight), 
  inverseMotorModel(inverseMotorModel) {}

void Robot::updateState(){
    double Vg = motorLeft.getSpeed();
    double Vd = motorRight.getSpeed();
    //Serial.print("Vg = "); Serial.print(Vg); Serial.print("; Vd = "); Serial.println(Vd);

    v = (Vg + Vd)/2;
    w = (Vd - Vg)/(2 * WIDTH);
    //Serial.print("v = "); Serial.print(v); Serial.print("; w = "); Serial.println(w);

    double dx = cos(theta) * v - LENGTH * sin(theta) * w;
    double dy = sin(theta) * v + LENGTH * cos(theta) * w;
    
    theta += w * DT * MS_TO_S;
    pos.x += DT * MS_TO_S * dx;
    pos.y += DT * MS_TO_S * dy;
    Serial.print("theta = "); Serial.print(theta); Serial.print("; x = "); Serial.print(pos.x); Serial.print("; y = "); Serial.println(pos.y);
}

void Robot::changeRef(const Point pos_ref){
    inverseMotorModel.setReference(pos_ref, pos);
    inverseMotorModel.update(theta);

    double Vd = inverseMotorModel.getVdStar();
    double Vg = inverseMotorModel.getVgStar();
    if (Vg > 300) Vg = 300;
    if (Vd > 300) Vd = 300;
    //Serial.println(Vg); Serial.println(Vd);
    motorControllerLeft.setControlledVoltage(Vg);
    motorControllerRight.setControlledVoltage(Vd);
    
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());


    //Serial.print("motorLeftVoltage = "); Serial.println(motorLeft.u);
    //Serial.print("motorRightVoltage = "); Serial.println(motorRight.u);

    motorLeft.applyVoltage();
    motorRight.applyVoltage();
}

void Robot::readMarvelmind(){
}

double Robot::getTheta(){
  return this->theta;
}

double Robot::getV(){
  return this->v; 
}

double Robot::getW(){
  return this->w; 
}

void Robot::init(){
    motorLeft.init();
    motorRight.init();
}