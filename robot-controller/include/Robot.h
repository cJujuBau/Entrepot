#pragma once

#include "MotorController.h"
#include "InverseMotorModel.h"
#include <math.h>

class Robot {
public:
    Robot(Point pos, Motor& motorLeft, Motor& motorRight, 
          MotorController& motorControllerLeft, MotorController& motorControllerRight, 
          InverseMotorModel& inverseMotorModel, double theta);
    void updateState();
    void readMarvelmind();
    void changeRef(const double Vx, const double Vy);
    void init();

    double getTheta();
    double getV();
    double getW();

    Motor& motorLeft;
    Motor& motorRight;
    MotorController& motorControllerLeft;
    MotorController& motorControllerRight;
    InverseMotorModel& inverseMotorModel;

private:
    Point pos;
    double theta;
    double v, w;
};