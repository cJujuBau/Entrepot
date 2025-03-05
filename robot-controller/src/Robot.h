#pragma once

class Robot {
public:
    Robot(Point pos, Motor motorLeft, Motor motorRight, MotorController motorControllerLeft, MotorController motorControllerRight, InverseMotorModel InverseMotorModel, double theta);
    void updateState();
    void readMarvelmind();
    double getTheta();

private:
    Point pos;
    Motor motorLeft;
    Motor motorRight;
    MotorController motorControllerLeft;
    MotorController motorControllerRight;
    InverseMotorModel inverseMotorModel;
    double theta;
    double v, w;
};