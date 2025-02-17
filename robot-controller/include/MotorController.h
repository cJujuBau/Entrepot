#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController {
public:
    MotorController();
    void init();
    void setSpeed(int speed);
    void stop();

private:
    int motorSpeed;
    bool motorState;
};

#endif // MOTORCONTROLLER_H