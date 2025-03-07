#include "src/InverseMotorModel.h"
#include "src/MotorController.h"
#include "src/Robot.h"
#include "src/utils/Utils.h"

#define SPEED 100.0
#define ROT_SPEED PI/4

struct Point pos_depart = {0, 0};
struct Point pos_ref = {250, 100};
struct Point vel_ref = {0, 0};

Motor motorLeft(34, 35, 12, 18, 31, FORWARD);
Motor motorRight(37, 36, 8, 19, 38, BACKWARD);

MotorController motorControllerLeft(50, 0, SPEED);
MotorController motorControllerRight(50, 0, SPEED);

InverseMotorModel inverseMotorModel(pos_depart, 0.5, 0.5);
Robot robot(pos_depart, motorLeft, motorRight, motorControllerLeft, motorControllerRight, inverseMotorModel, 0);

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  robot.motorLeft.init();
  robot.motorRight.init();
  
  attachInterrupt(digitalPinToInterrupt(motorLeft.getVA()), Motor::handleRisingEdge, RISING);
  attachInterrupt(digitalPinToInterrupt(motorRight.getVA()), Motor::handleRisingEdge, RISING);  
}

static long iteration = 0;

void loop() {
  iteration++;
  static long timePrec = 0;
  while (millis() - timePrec < DT) {}
  timePrec = millis();

  robot.inverseMotorModel.setReference(pos_ref);
  robot.inverseMotorModel.update(robot.motorControllerLeft, robot.motorControllerRight, robot.getTheta());
  
  //robot.motorControllerLeft.updateOutput(robot.motorLeft);
  //robot.motorControllerRight.updateOutput(robot.motorRight);
  
  if (iteration == 10) {
    Serial.println("+5v");
    motorRight.setVoltage(-3);
    motorLeft.setVoltage(3);
  } else if (iteration == 1000) {
    Serial.println("0 v");
     motorRight.setVoltage(0);
     motorLeft.setVoltage(0);
  }

  robot.motorLeft.applyVoltage();
  robot.motorRight.applyVoltage();

  // // Update variables
  // robot.updateState(); 
}  



