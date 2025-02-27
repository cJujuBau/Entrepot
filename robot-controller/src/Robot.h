#pragma once

class Robot {
public:
    Robot(MotorController leftMotor, MotorController rightMotor);
    void init();
    void setSpeed(float linearSpeed, float rotationalSpeed);
    void stop();

private:
    MotorController leftMotor;
    MotorController rightMotor;
    float length;
};