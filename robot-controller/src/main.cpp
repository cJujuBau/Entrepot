#include <iostream>
#include "wiringPi.h"
#include "MotorController.h"
#include "Robot.h"

using namespace std;

#define PI 3.1415

int main() {
    MotorController motorControllerLeft(37, 36, 8, FORWARD);
    MotorController motorControllerRight(34, 35, 12, BACKWARD);

    Robot robot(motorControllerLeft, motorControllerRight);

    float speed = 100; // Example speed
    float rotationalSpeed = PI/4;

    robot.init();

    // Control loop
    int i = 0;
    while (i++ < 4) {
        robot.setSpeed(speed, 0);

        delay(3000);
        robot.setSpeed(0, rotationalSpeed);
        delay(2000);
    }

    robot.stop();
    return 0;
}