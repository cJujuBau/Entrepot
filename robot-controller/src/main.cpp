#include <iostream>
#include "MotorController.h"

int main() {
    MotorController motorControllerLeft(1, FORWARD);
    MotorController motorControllerRight(1, BACKWARD);


    motorControllerLeft.init();
    motorControllerRight.init();

    int speed = 100; // Example speed
    motorControllerLeft.setSpeed(speed);
    motorControllerRight.setSpeed(speed);

    // Control loop (simplified)
    while (true) {
        // Here you could add logic to read sensors and adjust motor speed
        // For now, we'll just keep the motor running
        std::cout << "Motor running at speed: " << speed << std::endl;
    }

    motorController.stop();
    return 0;
}