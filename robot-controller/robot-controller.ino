#include "src/MotorController.h"
#include "src/Robot.h"
#include "src/utils/Utils.h"

#define SPEED 100.0
#define ROT_SPEED PI/4

Motor motorControllerLeft(34, 35, 12, 18, 31, BACKWARD);
Motor motorControllerRight(37, 36, 8, 19, 38, FORWARD);
Robot robot(Point(0,0), motorControllerLeft, motorControllerRight, 16.5);

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  robot.init();
}

void loop() {

}
