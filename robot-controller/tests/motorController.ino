#include "include/MotorController.h"
#include "include/Motor.h"
#include "include/Utils.h"

const double SPEED = 100.0; // mm.s-1
const double ROT_SPEED = PI/4.; // rad.s-1

const double Km = 1./50.;
const double Ki = 0.;

Motor motorRight(34, 35, 12, 18, 31, FORWARD);
Motor motorLeft(37, 36, 8, 19, 38, BACKWARD);

MotorController motorControllerLeft(Km, Ki);
MotorController motorControllerRight(Km, Ki);

void onRisingEdge_MD() {
  motorRight.onRisingEdge();
}

void onRisingEdge_MG() {
  motorLeft.onRisingEdge();
}

void setup() {
  // Initialize the robot
  Serial.begin(115200);
  motorLeft.init();
  motorRight.init();
  
  attachInterrupt(digitalPinToInterrupt(motorLeft.getVA()), onRisingEdge_MG, RISING);
  attachInterrupt(digitalPinToInterrupt(motorRight.getVA()), onRisingEdge_MD, RISING);
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

  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  if (elapsedTime <= 3000) {
    motorControllerLeft.setControlledVoltage(SPEED);
    motorControllerRight.setControlledVoltage(SPEED);
    if (forwardOnce++ == 0){
      Serial.println("Forward");
      Serial.println(motorControllerLeft.getControlledVoltage());
      Serial.println(motorControllerRight.getControlledVoltage());
    } 
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());

  } else if (elapsedTime <= 6000) {
    motorControllerLeft.setControlledVoltage(-SPEED);
    motorControllerRight.setControlledVoltage(SPEED);
    if (rightOnce++ == 0){
      Serial.println("Rotate Left");
      Serial.println(motorControllerLeft.getControlledVoltage());
      Serial.println(motorControllerRight.getControlledVoltage());
    } 
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());

  } else if (elapsedTime <= 9000) {
    (leftOnce++ > 0) ? : Serial.println("Rotate Right");
    motorControllerLeft.setControlledVoltage(SPEED);
    motorControllerRight.setControlledVoltage(-SPEED);
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());

  } else if (elapsedTime <= 12000) {
    (backwardOnce++ > 0) ? : Serial.println("Backward");
    motorControllerLeft.setControlledVoltage(-SPEED);
    motorControllerRight.setControlledVoltage(-SPEED);
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());

  } else if (elapsedTime <= 15000) {
    (stopOnce++ > 0) ? : Serial.println("Stop");
    motorControllerLeft.setControlledVoltage(0);
    motorControllerRight.setControlledVoltage(0);
    motorLeft.setVoltage(motorControllerLeft.getControlledVoltage());
    motorRight.setVoltage(motorControllerRight.getControlledVoltage());
  }

  motorLeft.applyVoltage();
  motorRight.applyVoltage();
}