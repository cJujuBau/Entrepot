#include "include/MotorController.h"
#include "include/Motor.h"
#include "include/Utils.h"

const double SPEED = 150.0; // mm.s-1
const double ROT_SPEED = PI/4.; // rad.s-1

const double Km = 1./50.;
const double Ki = 0.;

Motor motorRight(34, 35, 12, 18, 31, FORWARD);
Motor motorLeft(37, 36, 8, 19, 38, BACKWARD);

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

  if (elapsedTime <= 15000) {
    motorLeft.setVoltage(3);
    motorRight.setVoltage(0);
    
    if (forwardOnce++ == 0){
      Serial.println("Forward");
    }
  }
  motorLeft.applyVoltage();
  motorRight.applyVoltage();
}