#include "MotorController.h"
#include "utils/Utils.h"
#include "Arduino.h"

Motor* Motor::instance = nullptr;

Motor::Motor(int BI1, int BI2, int PWMB, int VA, int VB, int sens) : motorSpeed(0), BI1(BI1), BI2(BI2), PWMB(PWMB), sens(sens) {
    this->pulse = 0;
    this->pulsePrec = 0;
    this->tempsPrec = 0;
    this->u = 0;
    this->v = 0;
    instance = this;
}

void Motor::init() {
    // Initialization code for the motors
    pinMode(this->VA, INPUT_PULLUP);
    pinMode(this->VB, INPUT_PULLUP);

    pinMode(this->BI1, OUTPUT);
    pinMode(this->BI2, OUTPUT);
    pinMode(this->PWMB, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(this->VA), onRisingEdge, RISING);
}

float Motor::getSpeed() {
    updateSpeed();
    return v;
}

void Motor::updateSpeed(){
  long tps = millis();
  long dpulse = (pulse - pulsePrec);
  double dt = (tps - tempsPrec);
  double vit = WHEEL_RADIUS * dpulse * 2 * PI;
  vit = vit / (dt * MS_TO_S);
  vit = vit / (PULSE_PER_TURN * REDUCTION_RATE);

  pulsePrec = pulse;
  tempsPrec = tps;
  v = vit * sens;
}

void Motor::onRisingEdge(){
    if (instance) {
        instance->handleRisingEdge();
    }
}

void Motor::handleRisingEdge() {
  if (digitalRead(VB)) {
    pulse++;
  } else {
    pulse--;
  }
}

void Motor::setVoltage(const float voltage) {
    u = voltage * sens;
}

void Motor::applyVoltage(){
    float voltage = u;

    if (abs(voltage) < 1) { voltage = 0; }
    if (voltage > 0) {
        digitalWrite(BI2, LOW);
        digitalWrite(BI1, HIGH);
    } else {
        digitalWrite(BI1, LOW);
        digitalWrite(BI2, HIGH);
    }

    analogWrite(PWMB, convertToPWM(abs(voltage)));
}

MotorController::MotorController(double Km, double Ki, double ref) : Km(Km), Ki(Ki), ref(ref) {}

void MotorController::setReference(double ref) {
    this->ref = ref;
}

void MotorController::updateOutput(Motor &motor) {
    motor.setVoltage(this->ref / Km); // pas encore d'intégrateur implémenté
}