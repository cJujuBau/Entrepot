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
    void readGyro(float gyroTheta);
    void changeRef(const Point pos_ref);
    void init();

    double getTheta();
    double getV();
    double getW();
    Point getPos();

    Motor& motorLeft;
    Motor& motorRight;
    MotorController& motorControllerLeft;
    MotorController& motorControllerRight;
    InverseMotorModel& inverseMotorModel;

private:
    Point pos;
    double theta;
    double v, w;
    double Vg, Vd;
};