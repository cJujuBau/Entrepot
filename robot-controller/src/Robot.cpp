#include "MotorController.h"

class Robot {
public:
    Robot(MotorController leftMotor, MotorController rightMotor);
    void init();
    void setSpeed(int linearSpeed, int rotationalSpeed);
    void stop();

private:
    MotorController leftMotor;
    MotorController rightMotor;
};

Robot::Robot(MotorController leftMotor, MotorController rightMotor)
    : leftMotor(leftMotor), rightMotor(rightMotor) {}

void Robot::init() {
    leftMotor.init();
    rightMotor.init();
}

void Robot::setSpeed(int linearSpeed, int rotationalSpeed){
    leftMotor.setSpeed(speed);
}


void Robot::stop() {
    leftMotor.stop();
    rightMotor.stop();
}