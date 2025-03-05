#include "MotorController.h"
#include "Robot.h"


Robot::Robot(MotorController leftMotor, MotorController rightMotor, float length)
    : leftMotor(leftMotor), rightMotor(rightMotor), length(length) {}

void Robot::init() {
    leftMotor.init();
    rightMotor.init();
}

void Robot::setSpeed(float linearSpeed, float rotationalSpeed){
    leftMotor.setSpeed(linearSpeed - length * rotationalSpeed);
    rightMotor.setSpeed(linearSpeed + length * rotationalSpeed);
}

void Robot::stop() {
    leftMotor.stop();
    rightMotor.stop();
}