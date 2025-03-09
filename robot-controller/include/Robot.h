#pragma once

#include "MotorController.h"
#include "InverseMotorModel.h"
#include <math.h>

class Robot {
public:
    Robot(Point pos, Motor& motorLeft, Motor& motorRight, MotorController& motorControllerLeft, MotorController& motorControllerRight, InverseMotorModel& inverseMotorModel, double theta);
    void updateState();
    void readMarvelmind();
    double getTheta();

    Point pos;
    Motor& motorLeft;
    Motor& motorRight;
    MotorController& motorControllerLeft;
    MotorController& motorControllerRight;
    InverseMotorModel& inverseMotorModel;

private:
    double theta;
    double v, w;
};