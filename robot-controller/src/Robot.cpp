#include "../include/Robot.h"

extern int xMM, yMM;

Robot::Robot(Point pos, Motor& motorLeft, Motor& motorRight, MotorController& motorControllerLeft, 
             MotorController& motorControllerRight, InverseMotorModel& inverseMotorModel, double theta) 
: pos(pos), motorLeft(motorLeft), motorRight(motorRight), theta(theta), v(0), w(0), Vg(0), Vd(0),
  motorControllerLeft(motorControllerLeft), motorControllerRight(motorControllerRight), 
  inverseMotorModel(inverseMotorModel) {}

void Robot::updateState(){
    Vg = motorLeft.getSpeed();
    Vd = motorRight.getSpeed();
    //Serial.print("Vg = "); Serial.print(Vg); Serial.print("; Vd = "); Serial.println(Vd);

    v = (Vg + Vd)/2.;
    w = (Vd - Vg)/(2. * WIDTH);
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


    // robot alim is unable to handle too high values
    //if (Vg > 350) Vg = 350.;
    //if (Vd > 350) Vd = 350.;
    //if (Vg < -350) Vg = -350.;
    //if (Vd < -350) Vd = -350.;

    //Serial.println(Vg); Serial.println(Vd);
    motorControllerLeft.setControlledVoltage(inverseMotorModel.getVgStar(), Vg);
    motorControllerRight.setControlledVoltage(inverseMotorModel.getVdStar(), Vd);
    
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());


    //Serial.print("motorLeftVoltage = "); Serial.println(motorLeft.u);
    //Serial.print("motorRightVoltage = "); Serial.println(motorRight.u);

    motorLeft.applyVoltage();
    motorRight.applyVoltage();
}

void Robot::readMarvelmind(){
  pos.x = xMM;
  pos.y = yMM;
}

void Robot::readGyro(float gyroTheta){
  this->theta = gyroTheta;
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

Point Robot::getPos(){
  return this->pos;
}

void Robot::init(){
    motorLeft.init();
    motorRight.init();
}