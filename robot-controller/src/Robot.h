#pragma once

#include "utils/Utils.h"
#include "MotorController.h"
#include "InverseMotorModel.h"

class Robot {
public:
    Robot(struct Point pos, Motor& motorLeft, Motor& motorRight, MotorController& motorControllerLeft, MotorController& motorControllerRight, InverseMotorModel& inverseMotorModel, double theta);
    void updateState();
    void readMarvelmind();
    double getTheta();

    struct Point pos;
    Motor& motorLeft;
    Motor& motorRight;
    MotorController& motorControllerLeft;
    MotorController& motorControllerRight;
    InverseMotorModel& inverseMotorModel;

private:
    double theta;
    double v, w;
};