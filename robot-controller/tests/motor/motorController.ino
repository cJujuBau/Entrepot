#include "include/MotorController.h"
#include "include/Motor.h"
#include "include/Utils.h"

const double SPEED = 100.0; // mm.s-1
const double ROT_SPEED = PI/4; // rad.s-1

const double Km = 1/50;
const double Ki = 0;

Motor motorLeft(34, 35, 12, 18, 31, FORWARD);
Motor motorRight(37, 36, 8, 19, 38, BACKWARD);

MotorController motorControllerLeft(Km, Ki);
MotorController motorControllerRight(Km, Ki);

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  motorLeft.init();
  motorRight.init();
  
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

  //robot.motorControllerLeft.updateOutput(robot.motorLeft);
  //robot.motorControllerRight.updateOutput(robot.motorRight);


  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  if (elapsedTime <= 3000) {
    (forwardOnce++ > 0) ? : Serial.println("Forward");
    motorControllerLeft.setControlledVoltage(SPEED);
    motorControllerRight.setControlledVoltage(SPEED);

    motorRight.setVoltage(motorControllerLeft.getControlledVoltage());
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
  } else if (elapsedTime <= 6000) {
    (rightOnce++ > 0) ? : Serial.println("Rotate Right");
    motorControllerLeft.setControlledVoltage(-SPEED);
    motorControllerRight.setControlledVoltage(SPEED);

    motorRight.setVoltage(motorControllerLeft.getControlledVoltage());
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
  } else if (elapsedTime <= 9000) {
    (leftOnce++ > 0) ? : Serial.println("Rotate Left");
    motorControllerLeft.setControlledVoltage(SPEED);
    motorControllerRight.setControlledVoltage(-SPEED);

    motorRight.setVoltage(motorControllerLeft.getControlledVoltage());
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
  } else if (elapsedTime <= 12000) {
    (backwardOnce++ > 0) ? : Serial.println("Backward");
    motorControllerLeft.setControlledVoltage(-SPEED);
    motorControllerRight.setControlledVoltage(-SPEED);

    motorRight.setVoltage(motorControllerLeft.getControlledVoltage());
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
  } else if (elapsedTime <= 15000) {
    (stopOnce++ > 0) ? : Serial.println("Stop");
    motorControllerLeft.setControlledVoltage(0);
    motorControllerRight.setControlledVoltage(0);

    motorRight.setVoltage(motorControllerLeft.getControlledVoltage());
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
  }

  motorLeft.applyVoltage();
  motorRight.applyVoltage();

  // // Update variables
  // robot.updateState(); 
}



