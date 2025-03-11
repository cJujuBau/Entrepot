#include "include/Motor.h"

const double SPEED = 100.0;
const double ROT_SPEED = PI/4;

Motor motorLeft(34, 35, 12, 18, 31, FORWARD);
Motor motorRight(37, 36, 8, 19, 38, BACKWARD);

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

  motorLeft.applyVoltage();
  motorRight.applyVoltage();
}



