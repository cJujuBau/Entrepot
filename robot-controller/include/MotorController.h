#pragma once

#define FORWARD 1
#define BACKWARD -1

class MotorController {
public:
    MotorController(int BI1, int BI2, int PWMB, int base_direction);
    void init();
    void setSpeed(float speed);
    void stop();
    //int readSensor();
    void setDirection(int directionToSet);

private:
    int motorSpeed;
    int BI1; // BI1 pin number
    int BI2; // BI2 pin number
    int PWMB; // PWMB pin number
    int base_direction; // rotation direction (1 for direct, -1 for inverse). Depends on the motor physical implementation
    int direction;
}; 