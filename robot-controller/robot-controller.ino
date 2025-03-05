#include "src/MotorController.h"
#include "src/Robot.h"

#define SPEED 100.0
#define ROT_SPEED PI/4

MotorController motorControllerLeft(34, 35, 12, 18, 31, BACKWARD);
MotorController motorControllerRight(37, 36, 8, 19, 38, FORWARD);
Robot robot(motorControllerLeft, motorControllerRight, 16.5);

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  robot.init();
}

void loop() {
  // Control loop
  motorControllerRight.setSpeed(SPEED);
  delay(3000);

  motorControllerRight.setSpeed(0);
  delay(2000);
  // Stop the robot after the loop
  robot.stop();
}
