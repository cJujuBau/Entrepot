#include "include/InverseMotorModel.h"
#include "include/MotorController.h"
#include "include/Motor.h"
#include "include/Robot.h"
#include "include/Utils.h"

const double SPEED = 100.0;
const double ROT_SPEED = PI/4;

Point pos_depart = {0, 0};
Point pos_ref = {250, 100};
Point vel_ref = {0, 0};

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

static int forwardOnce = 0;
static int leftOnce = 0;
static int rightOnce = 0;
static int backwardOnce = 0;
static int stopOnce = 0;

void loop() {
  static long timePrec = 0;
  while (millis() - timePrec < DT) {}
  timePrec = millis();

  //robot.inverseMotorModel.setReference(pos_ref);
  //robot.inverseMotorModel.update(robot.motorControllerLeft, robot.motorControllerRight, robot.getTheta());
  
  //robot.motorControllerLeft.updateOutput(robot.motorLeft);
  //robot.motorControllerRight.updateOutput(robot.motorRight);
  
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  if (elapsedTime <= 3000) {
    (forwardOnce++ > 0) ? : Serial.println("Forward");;
    motorRight.setVoltage(3);
    motorLeft.setVoltage(3);
  } else if (elapsedTime <= 6000) {
    (rightOnce++ > 0) ? : Serial.println("Rotate Right");
    motorRight.setVoltage(3);
    motorLeft.setVoltage(-3);
  } else if (elapsedTime <= 9000) {
    (leftOnce++ > 0) ? : Serial.println("Rotate Left");
    motorRight.setVoltage(-3);
    motorLeft.setVoltage(3);
  } else if (elapsedTime <= 12000) {
    (backwardOnce++ > 0) ? : Serial.println("Backward");
    motorRight.setVoltage(-3);
    motorLeft.setVoltage(-3);
  } else if (elapsedTime <= 15000) {
    (stopOnce++ > 0) ? : Serial.println("Stop");
    motorRight.setVoltage(0);
    motorLeft.setVoltage(0);
  }

  robot.motorLeft.applyVoltage();
  robot.motorRight.applyVoltage();

  // // Update variables
  // robot.updateState(); 
}



