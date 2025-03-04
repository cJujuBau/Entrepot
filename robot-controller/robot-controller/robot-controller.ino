#include "../src/MotorController.h"
#include "../src/Robot.h"

#define PI 3.1415

MotorController motorControllerLeft(37, 36, 8, FORWARD);
MotorController motorControllerRight(34, 35, 12, BACKWARD);
Robot robot(motorControllerLeft, motorControllerRight);

void setup() {
  // Initialize the robot
  robot.init();
}

void loop() {
  float speed = 100; // Example speed
  float rotationalSpeed = PI / 4;

  // Control loop
  robot.setSpeed(speed, 0);
  delay(3000);
  robot.setSpeed(0, rotationalSpeed);
  delay(2000);

  // Stop the robot after the loop
  robot.stop();
}
